#include <gtest/gtest.h>
#include "GetFileCommand.h"
#include "IStorage.h"
#include "RLEDecompressor.h"
#include "IOutputChannel.h"

#include <string>
#include <unordered_map>
#include <sstream>
#include <vector>
#include <stdexcept>

using namespace std;

// Fake in-memory storage implementing IStorage
class FakeStorage : public IStorage {
public:
    void save(const string& file_name,
              const string& data) override {
        files_[file_name] = data;
    }

    string load(const string& file_name) const override {
        auto it = files_.find(file_name);
        if (it == files_.end()) {
            throw runtime_error("file not found");
        }
        return it->second;
    }

    void remove(const string& file_name) override {
        files_.erase(file_name);
    }

    vector<string> listFiles() const override {
        vector<string> result;
        result.reserve(files_.size());
        for (const auto& kv : files_) {
            result.push_back(kv.first);
        }
        return result;
    }

private:
    unordered_map<string, string> files_;
};

// Fake output channel to capture printed content
class FakeOutputChannel : public IOutputChannel {
public:
    void write(const std::string& data) override {
        buffer_ << data;
    }

    std::string getOutput() const {
        return buffer_.str();
    }

private:
    std::ostringstream buffer_;
};

// Prints decompressed content to output
TEST(GetFileCommandTest, PrintsDecompressedContentToOutput) {
    FakeStorage storage;
    const string file_name = "greeting.txt";

    // Compressed content (should decompress to "AAAB")
    const string compressed = "A3B1";
    storage.save(file_name, compressed);

    RLEDecompressor decompressor;
    FakeOutputChannel output;

    GetFileCommand command(storage, decompressor, output);

    vector<string> args = {file_name};
    command.execute(args);

    EXPECT_EQ("AAAB\n", output.getOutput());
}

// Works with empty file
TEST(GetFileCommandTest, WorksWithEmptyFile) {
    FakeStorage storage;
    const string file_name = "empty.txt";

    // Empty compressed data
    storage.save(file_name, "");

    RLEDecompressor decompressor;
    FakeOutputChannel output;

    GetFileCommand command(storage, decompressor, output);

    vector<string> args = {file_name};
    command.execute(args);

    // newline from the command
    EXPECT_EQ("\n", output.getOutput());
}
