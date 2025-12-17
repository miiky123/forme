#include <gtest/gtest.h>
#include "DeleteCommand.h"
#include "IStorage.h"
#include <string>
#include <unordered_map>
#include <stdexcept>
#include <vector>
#include "IOutputChannel.h"
#include <sstream>

using namespace std;

// Fake in-memory storage
class FakeStorage : public IStorage {
public:
    void save(const string& file_name, const string& data) override {
        files_[file_name] = data;
    }

    string load(const string& file_name) const override {
        auto it = files_.find(file_name);
        if (it == files_.end()) throw runtime_error("file not found");
        return it->second;
    }

    void remove(const string& file_name) override {
        if (files_.erase(file_name) == 0) {
            throw runtime_error("file not found");
        }
    }

    // Implement listFiles to satisfy IStorage interface
    std::vector<std::string> listFiles() const override {
        std::vector<std::string> result;
        result.reserve(files_.size());
        for (const auto& kv : files_) {
            result.push_back(kv.first);
        }
        return result;
    }

    bool fileExists(const string& file_name) const {
        return files_.count(file_name) > 0;
    }

private:
    unordered_map<string,string> files_;
};

// Fake Output Channel for capturing output
class FakeOutputChannel : public IOutputChannel {
public:
    void write(const std::string& data) override {
        buffer_ << data;
    }

    // Method to retrieve the captured output for assertion
    std::string getOutput() const {
        return buffer_.str();
    }

private:
    std::ostringstream buffer_;
};


// Successful deletion
TEST(DeleteCommandTest, DeletesExistingFile) {
    // Arrange
    FakeStorage storage;
    FakeOutputChannel output;
    storage.save("f.txt", "data");
    
    DeleteCommand cmd(storage, output);
    cmd.execute({"f.txt"});
    
    // Assert
    EXPECT_EQ(output.getOutput(), "204 No Content\n");
    EXPECT_FALSE(storage.fileExists("f.txt"));
}

// Deleting non-existing file → 404
TEST(DeleteCommandTest, DeleteNonExistentPrints404) {
    // Arrange
    FakeStorage storage;
    FakeOutputChannel output;
    
    DeleteCommand cmd(storage, output);
    cmd.execute({"nofile.txt"});
    
    // Assert
    EXPECT_EQ(output.getOutput(), "404 Not Found\n");
}

// Wrong number of args → exception (400 Bad Request)
TEST(DeleteCommandTest, WrongNumberOfArgsThrows) {
    // Arrange
    FakeStorage storage;
    FakeOutputChannel output;
    DeleteCommand cmd(storage, output);

    // Assert
    EXPECT_THROW(cmd.execute({}), invalid_argument);
    EXPECT_THROW(cmd.execute({"a", "b"}), invalid_argument);
    EXPECT_EQ(output.getOutput(), ""); 
}

// Only the right file is deleted
TEST(DeleteCommandTest, OnlyDeletesSpecifiedFile) {
    // Arrange
    FakeStorage storage;
    FakeOutputChannel output;
    storage.save("toDelete.txt", "A1");
    storage.save("keep.txt", "B1");

    DeleteCommand cmd(storage, output);
    cmd.execute({"toDelete.txt"});
    
    // Assert
    EXPECT_EQ(output.getOutput(), "204 No Content\n");
    EXPECT_FALSE(storage.fileExists("toDelete.txt"));
    EXPECT_TRUE(storage.fileExists("keep.txt"));
}
