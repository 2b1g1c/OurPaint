cmake_minimum_required(VERSION 3.14)
project(ourpainttest)

# GoogleTest requires at least C++14
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include(FetchContent)
if (NOT TARGET gtest)
    FetchContent_Declare(
            googletest
            GIT_REPOSITORY https://github.com/google/googletest.git
            GIT_TAG 03597a01ee50ed33e9dfd640b249b4be3799d395
    )
    set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
    FetchContent_MakeAvailable(googletest)
endif()

enable_testing()
set(SOURCES
        ../src/objects.cpp
        ../src/requirements.cpp
        ../src/saveload/objectInFile.cpp
        ../src/saveload/FileOurP.cpp
        ../src/saveload/requirementInFile.cpp
    )
add_executable(
  savergtest
  savergtests.cpp
  ${SOURCES}
)
include_directories(../math)

target_link_libraries(savergtest MinimizerOptimizer)

target_link_libraries(
        savergtest
        GTest::gtest_main
)

add_executable(
        addReqInComponentGtest
        addReqInComponentTests.cpp
        ${SOURCES}
        addReqInComponentTests.cpp)

target_link_libraries(
        addReqInComponentGtest
        GTest::gtest_main
)

include(GoogleTest)

gtest_discover_tests(savergtest)

gtest_discover_tests(addReqInComponentGtest)