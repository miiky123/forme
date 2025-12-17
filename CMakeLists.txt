cmake_minimum_required(VERSION 3.14)

# Do not run tests automatically 
set(CMAKE_VS_INCLUDE_RUN_TESTS_IN_DEFAULT_BUILD OFF)

project(Project_MKH)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

#  Application target 

# All .cpp files under src/
file(GLOB APP_SOURCES
    "${CMAKE_SOURCE_DIR}/src/*.cpp"
)

add_executable(project_mkh ${APP_SOURCES})
target_include_directories(project_mkh PUBLIC "${CMAKE_SOURCE_DIR}/src")

# - GoogleTest and tests

enable_testing()

include(FetchContent)
FetchContent_Declare(
    gtest
    GIT_REPOSITORY https://github.com/google/googletest.git
    GIT_TAG v1.14.0
)
FetchContent_MakeAvailable(gtest)

add_subdirectory(tests)