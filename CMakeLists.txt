cmake_minimum_required(VERSION 3.10)
project(MyList)

file(GLOB SRCS "*.cpp")

if(SRCS)
    add_executable(code ${SRCS})
else()
    # If no cpp files, create a dummy one so cmake doesn't fail
    file(WRITE dummy.cpp "int main() { return 0; }")
    add_executable(code dummy.cpp)
endif()
