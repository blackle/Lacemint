cmake_minimum_required(VERSION 2.8.9)
project (lacemint)

include_directories(src)
file(GLOB_RECURSE SOURCES "src/*.cpp")

add_executable(lacemint ${SOURCES})

set(CMAKE_CXX_FLAGS "-std=c++11")
set(CMAKE_EXE_LINKER_FLAGS "-lunicorn -lm -lpthread")
