cmake_minimum_required(VERSION 3.5)
project(octolink)

set(CMAKE_CXX_STANDART 20)

add_executable(octolink src/server.cpp)

find_package(Boost REQUIRED COMPONENTS system)

target_link_libraries(octolink Boost::system pthread)
