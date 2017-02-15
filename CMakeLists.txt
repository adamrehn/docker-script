cmake_minimum_required(VERSION 3.5)
project(docker-script)

# Set the C++ standard to C++11
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Enable all compiler warnings
if (MSVC)
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /W4")
else()
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall")
endif()

# Build the main executable
add_executable(docker-script
	source/docker-script.cpp
	source/NvidiaDockerImages.cpp
	source/Utility.cpp)

# Installation rules
# (Install using `cmake --build . --target install`)
install(TARGETS docker-script DESTINATION bin)
