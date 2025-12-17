# tests/CMakeLists.txt

# 1. Collect all test source files in tests
file(GLOB TEST_SOURCES
    "${CMAKE_CURRENT_SOURCE_DIR}/*.cpp"
)

# 2. Point to the src directory
set(SRC_DIR "${CMAKE_CURRENT_SOURCE_DIR}/../src")

# 3. Collect all implementation .cpp files from src/
file(GLOB CORE_SOURCES
    "${SRC_DIR}/*.cpp"
)

# 4. Exclude main.cpp from the tests (prevent a second main() inside unit_tests)
list(FILTER CORE_SOURCES EXCLUDE REGEX ".*/main\\.cpp$")

# 5. Create the unit_tests executable from:
#    all the test .cpp files
#    all the implementation .cpp 
add_executable(unit_tests
    ${TEST_SOURCES}
    ${CORE_SOURCES}
)

# 6. Let tests see headers from src/
target_include_directories(unit_tests PUBLIC "${SRC_DIR}")

# 7. Link GoogleTest
target_link_libraries(unit_tests PRIVATE gtest gtest_main)

# 8. Register the test executable with CTest
add_test(
    NAME AllTests
    COMMAND $<TARGET_FILE:unit_tests>
)
