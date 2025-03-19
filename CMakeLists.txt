cmake_minimum_required(VERSION 3.10)
project(TaskQueue)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)
set(SRC_FILES 
	main.cpp 
	Logger.cpp
)

# Add source files
add_executable(log ${SRC_FILES})

# link to pthread
if(UNIX)
    target_link_libraries(log pthread)
endif()
