cmake_minimum_required(VERSION 3.10)
set(CMAKE_CXX_STANDARD 17)

#Specify the project
project(DMProject)
add_executable(${PROJECT_NAME} main.cpp)


#Add Matrix directory as a library because of the templates. 
#http://mariobadr.com/creating-a-header-only-library-with-cmake.html
add_library(Matrix INTERFACE)
target_include_directories(Matrix INTERFACE Matrix)

#Add our linear_regression library.
#https://www.youtube.com/watch?v=kEGQKzhciKc
add_subdirectory(Linear_regression)
target_link_libraries(${PROJECT_NAME} LINK_PUBLIC Linear_regression)
