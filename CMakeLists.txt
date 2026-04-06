cmake_minimum_required(VERSION 3.20)
project(ink)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Boost REQUIRED COMPONENTS system)
find_package(nlohmann_json REQUIRED)
find_package(PostgreSQL REQUIRED)

#=========
# C o r e
#=========

add_library(core STATIC
        src/models/client.cpp
        src/models/client_manager.cpp
        src/net/fishman.cpp
        src/net/protocol.cpp
        src/tools.cpp
        src/crypto/password.cpp
)

target_include_directories(core PUBLIC
        include
        ${PostgreSQL_INCLUDE_DIRS}
)

target_link_libraries(core PUBLIC
        Boost::system
        nlohmann_json::nlohmann_json
        crypt
        ${PostgreSQL_LIBRARIES}
)

#=============
# S e r v e r
#=============

add_executable(server
        src/main.cpp
        src/net/server.cpp
        src/db/database.cpp
)

target_include_directories(server PRIVATE
        include
        ${PostgreSQL_INCLUDE_DIRS}
)

target_link_libraries(server PRIVATE
        core
        pthread
        ${PostgreSQL_LIBRARIES}
)

#=============
# C l i e n t
#=============

add_executable(client
        src/client_app.cpp
        src/db/database.cpp
        src/crypto/password.cpp
)

target_include_directories(client PRIVATE
        include
        ${PostgreSQL_INCLUDE_DIRS}
)

target_link_libraries(client PRIVATE
        core
        pthread
        ${PostgreSQL_LIBRARIES}
)

#===========
# T e s t s
#===========

if(INK_BUILD_TESTS)
    enable_testing()

    include(FetchContent)
    FetchContent_Declare(
            googletest
            URL https://github.com/google/googletest/archive/refs/tags/v1.14.0.zip
    )
    FetchContent_MakeAvailable(googletest)

    add_library(core_test STATIC
            src/models/client.cpp
            src/models/client_manager.cpp
            src/net/fishman.cpp
            src/net/protocol.cpp
            src/tools.cpp
    )
    target_compile_definitions(core_test PRIVATE INK_TEST_MODE)
    target_include_directories(core_test PUBLIC include)
    target_link_libraries(core_test PUBLIC
            Boost::system
            nlohmann_json::nlohmann_json
    )

    add_executable(tests
            tests/test_protocol.cpp
            tests/test_manager.cpp
    )

    target_compile_definitions(tests PRIVATE INK_TEST_MODE)

    target_link_libraries(tests
            core_test
            gtest
            gtest_main
            pthread
    )

    include(GoogleTest)
    gtest_discover_tests(tests)
endif()