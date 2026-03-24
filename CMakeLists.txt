cmake_minimum_required(VERSION 3.5)
project(octolink)

set(CMAKE_CXX_STANDART 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Boost REQUIRED COMPONENTS system)

include_directories(include)

add_executable(octolink_server src/server.cpp src/client.cpp)
add_executable(octolink_client src/client_app.cpp)

target_link_libraries(octolink_server Boost::system pthread)
target_link_libraries(octolink_client Boost::system pthread)