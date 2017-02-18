/*
	docker-script: run script files inside Docker containers
	Copyright (c) 2017, Adam Rehn
	
	Permission is hereby granted, free of charge, to any person obtaining a copy
	of this software and associated documentation files (the "Software"), to deal
	in the Software without restriction, including without limitation the rights
	to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
	copies of the Software, and to permit persons to whom the Software is
	furnished to do so, subject to the following conditions:
	
	The above copyright notice and this permission notice shall be included in all
	copies or substantial portions of the Software.
	
	THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
	IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
	FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
	AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
	LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
	OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
	SOFTWARE.
	
*/
#include "NvidiaDockerImages.h"
#include "Utility.h"
#include <stdexcept>
#include <iostream>
#include <fstream>
#include <cstdlib>
#include <vector>
using std::ifstream;
using std::vector;
using std::clog;
using std::endl;

int main (int argc, char* argv[])
{
	//Check that a script file was specified
	if (argc > 1)
	{
		try
		{
			//Check if verbose output was requested
			bool verboseOutput = (argc > 2 && string(argv[2]) == "---verbose");
			
			//Gather all remaining CLI arguments, so they can be passed to the script
			string trailingArgs = "";
			for (int i = ((verboseOutput == true) ? 3 : 2); i < argc; ++i) {
				trailingArgs += "\"" + string(argv[i]) + "\" ";
			}
			
			//Attempt to open the script file
			string scriptPath = argv[1];
			ifstream script(scriptPath.c_str());
			if (script.is_open() == false) {
				throw std::runtime_error("failed to open script file \"" + scriptPath + "\"");
			}
			
			//Attempt to read the second shebang line in the script file
			string shebang = "";
			std::getline(script, shebang);
			std::getline(script, shebang);
			if (script.fail()) {
				throw std::runtime_error("failed to read second shebang line from \"" + scriptPath + "\"");
			}
			
			//Verify that the shebang line is well-formed
			size_t spacePos = shebang.find(" ");
			if (shebang.substr(0,2) != "#!" || spacePos == string::npos) {
				throw std::runtime_error("invalid second shebang line in script file \"" + scriptPath + "\"");
			}
			
			//Parse the shebang line
			string dockerImage = shebang.substr(2, spacePos - 2);
			string interpreter = shebang.substr(spacePos + 1);
			
			//Determine the full path to the script file
			scriptPath = Utility::realPath(scriptPath);
			
			//Extract the directory and filename components of the script's path, as well as our current working directory
			string scriptFile = Utility::baseName(scriptPath);
			string scriptDir  = Utility::dirName(scriptPath);
			string workingDir = Utility::cwd();
			
			//Extract the docker image name, without trailing tag name
			string imageWithoutTag = dockerImage.substr(0, dockerImage.find(":"));
			
			//If the specified docker image (or the specific image and tag combination) requires
			//nvidia-docker, we invoke nvidia-docker instead of regular docker
			string docker = "docker";
			auto nvDockerImages = NvidiaDockerImages::getList();
			if (Utility::contains(nvDockerImages, imageWithoutTag) || Utility::contains(nvDockerImages, dockerImage)) {
				docker = "nvidia-docker";
			}
			
			//Build the `docker run` command
			string command = docker + " run " +
				string("\"-v") + scriptDir + ":/scriptdir\" " +
				string("\"-v") + workingDir + ":/workingdir\" " +
				string("--workdir=/workingdir ") +
				string("-e \"HOST_CWD=") + workingDir + "\" " +
				string("-ti --rm --entrypoint=\"\" ") +
				string("\"") + dockerImage + "\" " +
				string("\"") + interpreter + "\" " +
				string("\"/scriptdir/") + scriptFile + "\" " + trailingArgs;
			
			//If verbose output was requested, display diagnostic information
			if (verboseOutput == true)
			{
				clog << "Docker image:       " << dockerImage << endl;
				clog << "Interpreter:        " << interpreter << endl;
				clog << "Script path:        " << scriptPath << endl;
				clog << "Script basename:    " << scriptFile << endl;
				clog << "Script dirname:     " << scriptDir << endl;
				clog << "Working directory:  " << workingDir << endl;
				clog << "Trailing arguments: " << trailingArgs << endl;
				clog << endl << "Docker command:" << endl << command << endl << endl;
			}
			
			//Invoke docker
			system(command.c_str());
		}
		catch (std::runtime_error& e)
		{
			clog << "Error: " << e.what() << endl << endl;
			return 1;
		}
	}
	else
	{
		clog << "Usage:" << endl << argv[0] << " <SCRIPT> [---verbose] [args for script]" << endl << endl;
		clog << "The first line of the script file should be a normal Unix shebang line." << endl;
		clog << "The second line of the script file should be:" << endl;
		clog << "#!<IMAGE> <INTERPRETER>" << endl << endl;
	}
	
	return 0;
}