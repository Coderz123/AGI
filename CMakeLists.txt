cmake_minimum_required(VERSION 3.13)  # CMake version check
project(AGI)		               # Create project "simple_example"
set(CMAKE_CXX_STANDARD 20)            # Enable c++14 standard

# Add main.cpp file of the project root directory as a source file
set(SOURCE_FILES "./src/AGI.cpp")

find_package(Crow)
target_link_libraries(Crow::Crow)


# Add executable target with source files listed in SOURCE_FILES variable
add_executable(AGI ${SOURCE_FILES})

#add_custom_command(TARGET AGI POSTBUILD cp AGI .. VERBATIM)
#add_custom_command(TARGET AGI POSTBUILD AGI WORKING_DIRECTORY .. VERBATIM)
