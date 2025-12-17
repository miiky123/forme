#include <gtest/gtest.h>
#include "FileStorage.h"
#include <string>
#include <stdexcept>
#include <cstdio>   // std::remove

using namespace std;

TEST(FileStorageTest, SaveAndLoadSimpleContent) {
    // 1. Save a file and load it back
    const string baseDir = ".";
    const string fileName = "test_file_1.txt";
    const string content = "Hello World";

    // Clean before test
    remove((baseDir + "/" + fileName).c_str());

    FileStorage storage(baseDir);

    storage.save(fileName, content);
    EXPECT_EQ(storage.load(fileName), content);

    // Clean after test
    remove((baseDir + "/" + fileName).c_str());
}

TEST(FileStorageTest, OverwriteExistingFile) {
    // 2. Saving again should overwrite previous content
    const string baseDir = ".";
    const string fileName = "test_file_2.txt";

    // Clean before
    remove((baseDir + "/" + fileName).c_str());

    FileStorage storage(baseDir);

    const string first = "First Version";
    const string second = "Second Version";

    storage.save(fileName, first);
    storage.save(fileName, second);

    EXPECT_EQ(storage.load(fileName), second);

    // Clean after
    remove((baseDir + "/" + fileName).c_str());
}

TEST(FileStorageTest, EmptyContentIsSavedAndLoaded) {
    // 3. Saving empty content should be valid
    const string baseDir = ".";
    const string fileName = "empty_test.txt";

    remove((baseDir + "/" + fileName).c_str());

    FileStorage storage(baseDir);

    const string content = "";
    storage.save(fileName, content);

    EXPECT_EQ(storage.load(fileName), content);

    remove((baseDir + "/" + fileName).c_str());
}

TEST(FileStorageTest, LoadNonExistingFileThrows) {
    // 4. Loading a missing file must throw an exception
    const string baseDir = ".";
    const string fileName = "non_existent_abc.txt";

    remove((baseDir + "/" + fileName).c_str());

    FileStorage storage(baseDir);

    EXPECT_THROW(storage.load(fileName), std::runtime_error);
}
