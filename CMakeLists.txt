cmake_minimum_required(VERSION 3.10)
project(RedChannelExtractor)

# Specify C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Find OpenCV
find_package(OpenCV REQUIRED)

# Include OpenCV headers
include_directories(${OpenCV_INCLUDE_DIRS})

# Add the executable
add_executable(RedChannelExtractor test.cpp angleDetection.cpp)

# Link OpenCV libraries
target_link_libraries(RedChannelExtractor ${OpenCV_LIBS})
