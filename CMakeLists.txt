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

# Under non-Windows platforms, we support the nvidia-docker-script symlink
if (!WIN32)
	
	# Create the symlink when the docker-script executable is built
	add_custom_command(
		TARGET docker-script
		POST_BUILD
		COMMAND ${CMAKE_COMMAND} -E create_symlink ${CMAKE_INSTALL_PREFIX}/bin/docker-script nvidia-docker-script
	)
	
	# Copy the symlink at install time
	install(FILES ${CMAKE_CURRENT_BINARY_DIR}/nvidia-docker-script DESTINATION bin)
	
endif()
