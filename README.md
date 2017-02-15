# `docker-script`: run script files inside Docker containers

`docker-script` uses the Unix shebang system to make it easy to run script files inside Docker containers, by automating the details of interacting with Docker.

The key features of `docker-script` are:

- Automatically spins up containers, runs scripts inside them with any supplied command-line arguments, and performs cleanup
- When running a script, automatically binds both the script's containing directory and the current working directory to mount points inside the container, allowing the script to interact with the host filesystem
- Automatically invokes [NVIDIA Docker](https://github.com/NVIDIA/nvidia-docker) for images that require it (see [`NvidiaDockerImages.cpp`](./source/NvidiaDockerImages.cpp) for the list of supported images)

**The latest binaries are available from the [releases page](https://github.com/adamrehn/docker-script/releases).**


## Table of contents

- [Usage](#usage)
- [Building from source](#building-from-source)
- [Shebang support under Windows](#shebang-support-under-windows)
- [Bind-mounting considerations under Windows and macOS](#bind-mounting-considerations-under-windows-and-macos)
- [NVIDIA Docker Images](#nvidia-docker-images)


## Usage

The file `test.py` provides an example of a Python script that uses `docker-script`. The first two lines of the file look like this:

```
#!/usr/bin/env docker-script
#!python:latest python
```

The first line is the standard Unix shebang, and the second line specifies the name of the Docker image (with optional tag) that the script is designed to be run inside, and the command for the interpreter that will run the script. The reason for this two-line split is to maximise cross-platform compatibility by avoiding reliance on specific behaviour of the shebang mechanism, which can vary significantly between implementations (see http://stackoverflow.com/a/4304187 for details.)

By using the `---verbose` option, we can see what command `docker-script` will invoke when it is called with `test.py`. Assuming that the test Python script is located at `/tmp/docker-script/test.py`, and that we run the following commands:

```
cd /tmp
chmod a+x ./docker-script/test.py
./docker-script/test.py ---verbose arg1 arg2 'argument 3'
```

The verbose output from `docker-script` will show us the generated command that will be used to run Docker:

```
docker run \
 "-v/tmp/docker-script:/scriptdir" \
 "-v/tmp:/workingdir" \
 --workdir=/workingdir \
 -e "HOST_CWD=/tmp" \
 -ti --rm \
 "python:latest" "python" \
 "/scriptdir/test.py" "arg1" "arg2" "argument 3"
```

If all goes well, output should be displayed from the Python script that is similar to the following:

```
Uname:     Linux 4.9.4-moby x86_64
Guest CWD: /workingdir
Host CWD:  /tmp
argv:      ['/scriptdir/test.py', 'arg1', 'arg2', 'argument 3']
```


## Building from source

Building requires [CMake](https://cmake.org/), and follows the standard CMake build process, as shown below. (Under Windows, the commands will need to be run from a Visual Studio Developer Command Prompt.)

```
mkdir build && cd build
cmake ..
cmake --build .
```


## Shebang support under Windows

Although Windows doesn't support the Unix shebang mechanism natively, there are a number of options for overcoming this limitation.

### Option A: Use a Windows port of Bash

There are a number of ports of the `bash` shell for Windows. **Git Bash**, which is bundled with [Git for Windows](https://git-for-windows.github.io/), is one of the easiest to install. From inside the `bash` shell, the Unix shebang mechanism will work exactly as it does on other platforms.

### Option B: Run the Docker client through WSL

The **Windows Subsystem for Linux (WSL)** allows native Linux binaries to run under Windows. Although the Docker daemon itself cannot run under WSL at the time of writing, the Docker client can, and can communicate with the Docker daemon over a local TCP socket. The setup for this is quite complicated, and is outside the scope of this README. This option won't provide any benefits to users who wish to use `docker-script` as part of a workflow involving native Windows tools, and is only useful for users already utilising a Linux-based workflow under WSL who wish to integrate `docker-script` into that.

### Option C: Ignore the shebang entirely

If you don't want to use the shebang mechanism at all, you can simply pass a script file directly as an argument to `docker-script` from the command line:

```
docker-script test.py arg1 arg2 "argument 3"
```

Note that `docker-script` still expects the first line of the file to be a shebang, but its contents are ignored, and only the custom shebang on the second line (which specifies the Docker image and interpreter) will actually be used.


## Bind-mounting considerations under Windows and macOS

When running Docker under macOS, and when running Linux-based Docker images under Windows, the Docker daemon itself resides in a Linux Virtual Machine (VM). As a consequence, only paths that have been shared from the host to the VM can be bind-mounted to containers. See the relevant documentation for each platform:

- [Docker for Windows: Shared Drives](https://docs.docker.com/docker-for-windows/#/shared-drives)
- [Docker for Mac: File system sharing](https://docs.docker.com/docker-for-mac/osxfs/#namespaces)


## NVIDIA Docker images

When a script specifies a Docker image and tag combination that requires [NVIDIA Docker](https://github.com/NVIDIA/nvidia-docker) to run, the generated command will automatically use `nvidia-docker` instead of regular `docker`. **Note that at the time of writing, NVIDIA Docker is only available under Linux.**

The full list of supported image and tag combinations can be found in [`NvidiaDockerImages.cpp`](./source/NvidiaDockerImages.cpp), but a few key ones are listed below.

**NVIDIA images:**

- [nvidia/cuda](https://hub.docker.com/r/nvidia/cuda/)
- [nvidia/caffe](https://hub.docker.com/r/nvidia/caffe/)
- [nvidia/digits](https://hub.docker.com/r/nvidia/digits/)

**Third-party images:**

- [bvlc/caffe:gpu](https://hub.docker.com/r/bvlc/caffe/)
- [dmlc/mxnet:cuda](https://hub.docker.com/r/dmlc/mxnet/)
- [kaixhin/cuda-theano](https://hub.docker.com/r/kaixhin/cuda-theano/)
- [microsoft/cntk:latest](https://hub.docker.com/r/microsoft/cntk/)
- [tensorflow/tensorflow:latest-gpu](https://hub.docker.com/r/tensorflow/tensorflow/)
