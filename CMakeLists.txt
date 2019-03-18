cmake_minimum_required(VERSION 3.0)

project(bwr_router)

file(GLOB files
    "*.h"
    "*.cpp"
)

add_executable(bwr_router ${files})
