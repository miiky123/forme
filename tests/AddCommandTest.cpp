#include <gtest/gtest.h>

#include "AddCommand.h"
#include "IStorage.h"
#include "RLECompressor.h"
#include "IOutputChannel.h"

#include <string>
#include <unordered_map>
#include <stdexcept>
#include <vector>

using std::string;
using std::unordered_map;

// Fake storage for tests
class FakeStorage : public IStorage {
public:
    void save(const string& file_name,
              const string& data) override {
        db[file_name] = data;
    }

    string load(const string& file_name) const override {
        auto it = db.find(file_name);
        if (it == db.end()) {
            throw std::runtime_error("File not found: " + file_name);
        }
        return it->second;
    }

    std::vector<std::string> listFiles() const override {
        std::vector<std::string> result;
        for (const auto& kv : db) {
            result.push_back(kv.first);
        }
        return result;
    }

    void remove(const std::string& file_name) override {
        // Simple implementation for tests: remove from the in-memory map
        db.erase(file_name);
    }

    unordered_map<string, string> db;
};

// Fake output channel for tests
class FakeOutputChannel : public IOutputChannel {
public:
    std::string buffer;

    void write(const std::string& data) override {
        buffer += data;
    }
};

TEST(AddCommandTest, SavesCompressedDataToStorage) {
    FakeStorage storage;
    RLECompressor compressor;
    FakeOutputChannel output;

    // Updated constructor: storage + compressor + output
    AddCommand cmd(storage, compressor, output);

    // Args: args[0] = file name, args[1] = text
    std::vector<std::string> args = {"file1.txt", "AAABB"};
    cmd.execute(args);

    EXPECT_EQ(storage.load("file1.txt"), compressor.compress("AAABB"));
}

TEST(AddCommandTest, HandlesContentWithSpaces) {
    FakeStorage storage;
    RLECompressor compressor;
    FakeOutputChannel output;

    AddCommand cmd(storage, compressor, output);

    // Text with spaces – everything after the file name
    std::vector<std::string> args = {"note.txt", "HELLO WORLD"};
    cmd.execute(args);

    EXPECT_EQ(storage.load("note.txt"), compressor.compress("HELLO WORLD"));
}
