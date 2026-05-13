cmake_minimum_required(VERSION 3.19)
project(alice-sandbox-calc)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)

include_directories(${PROJECT_SOURCE_DIR}/include)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(COMMON_WARNINGS_MSVC /W4 /WX /permissive-)
set(COMMON_WARNINGS_GCC -Wall -Wextra -Wpedantic -pedantic-errors -Werror)

find_program(CLANG_TIDY_EXE NAMES clang-tidy)

set(PROJECT_SOURCES
    src/dungeon.cpp
    src/parser.cpp
    src/alice-bot.cpp
    src/simulator.cpp
)

function(apply_common_settings TARGET_NAME)
    target_compile_options(${TARGET_NAME} PRIVATE
        $<$<CXX_COMPILER_ID:MSVC>:${COMMON_WARNINGS_MSVC}>
        $<$<NOT:$<CXX_COMPILER_ID:MSVC>>:${COMMON_WARNINGS_GCC}>
    )

    if(CMAKE_BUILD_TYPE STREQUAL "Debug")
        if(CLANG_TIDY_EXE)
            set_target_properties(${TARGET_NAME} PROPERTIES
                CXX_CLANG_TIDY "${CLANG_TIDY_EXE};-header-filter=^${PROJECT_SOURCE_DIR}/"
            )
        endif()

        if(CMAKE_CXX_COMPILER_ID MATCHES "Clang|GNU" AND NOT WIN32 AND NOT APPLE)
            target_compile_options(${TARGET_NAME} PRIVATE
                -fsanitize=address,undefined -fno-omit-frame-pointer)
            target_link_options(${TARGET_NAME} PRIVATE
                -fsanitize=address,undefined)
        endif()
    endif()
endfunction()

# Main
add_executable(alice-sandbox-calc src/main.cpp ${PROJECT_SOURCES})
apply_common_settings(alice-sandbox-calc)


# GTest
find_package(GTest QUIET)
if(NOT GTest_FOUND)
    message(STATUS "GTest not found on system, downloading via FetchContent...")
    include(FetchContent)
    FetchContent_Declare(
        googletest
        GIT_REPOSITORY https://github.com/google/googletest.git
        GIT_TAG        v1.14.0
    )
    set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
    FetchContent_MakeAvailable(googletest)
endif()

enable_testing()

function(add_gtest TEST_NAME TEST_SRC)
    add_executable(${TEST_NAME} ${TEST_SRC} ${PROJECT_SOURCES})
    target_link_libraries(${TEST_NAME} GTest::gtest_main)
    apply_common_settings(${TEST_NAME})
    add_test(NAME ${TEST_NAME} COMMAND ${TEST_NAME})
endfunction()

add_gtest(resource-test   test/resource-test.cpp)
add_gtest(dungeon-test    test/dungeon-test.cpp)
add_gtest(parser-test     test/parser-test.cpp)
add_gtest(simulator-test  test/simulator-test.cpp)
add_gtest(alice-bot-test  test/alice-bot-test.cpp)

add_executable(main-test test/main-test.cpp)
target_link_libraries(main-test GTest::gtest_main)
apply_common_settings(main-test)
add_dependencies(main-test alice-sandbox-calc)
add_test(NAME main-test COMMAND main-test
    WORKING_DIRECTORY $<TARGET_FILE_DIR:alice-sandbox-calc>)

add_custom_target(execute-tests
    COMMAND env GTEST_COLOR=1 ${CMAKE_CTEST_COMMAND} --verbose --output-on-failure
    DEPENDS resource-test dungeon-test parser-test simulator-test alice-bot-test main-test
)
