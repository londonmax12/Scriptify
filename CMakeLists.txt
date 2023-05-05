cmake_minimum_required(VERSION "3.24.0")

project("Scriptify")

set(CMAKE_CXX_STANDARD 17)

add_executable("${PROJECT_NAME}"
"${PROJECT_NAME}/src/dataHandler.h"
"${PROJECT_NAME}/src/data.h"
"${PROJECT_NAME}/src/dataHandler.cpp"
"${PROJECT_NAME}/src/data.cpp"
)

target_include_directories(${PROJECT_NAME} PRIVATE ${PROJECT_NAME}/include)