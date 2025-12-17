#include <gtest/gtest.h>
#include "SearchCommand.h"
#include "IStorage.h"
#include "RLEDecompressor.h"
#include "IOutputChannel.h"

#include <string>
#include <unordered_map>
#include <sstream>
#include <stdexcept>
#include <vector>
#include <algorithm>

using namespace std;

// Simple in-memory storage for tests implementing IStorage
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

    void remove(const std::string& file_name) override {
        files_.erase(file_name);
    }

    std::vector<std::string> listFiles() const override {
        std::vector<std::string> result;
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

// Helper: split output into words, ignoring whitespace/newlines
static vector<string> splitWords(const string& s) {
    vector<string> words;
    istringstream iss(s);
    string w;
    while (iss >> w) {
        words.push_back(w);
    }
    return words;
}

// 1. Single matching file
TEST(SearchCommandTest, PrintsSingleMatchingFileName) {
    FakeStorage storage;

    storage.save("file1.txt", "A3B1");   // AAAB
    storage.save("file2.txt", "C2D2");   // CCDD

    RLEDecompressor decompressor;
    FakeOutputChannel output;

    SearchCommand command(storage, decompressor, output);

    std::vector<std::string> args = {"AAB"};
    command.execute(args);

    vector<string> words = splitWords(output.getOutput());
    ASSERT_EQ(1u, words.size());
    EXPECT_EQ("file1.txt", words[0]);
}

// 2. Multiple matching files
TEST(SearchCommandTest, PrintsMultipleMatchingFileNames) {
    FakeStorage storage;

    storage.save("file1.txt", "A3B1");      // AAAB
    storage.save("file2.txt", "A1X1A2");    // AXAA
    storage.save("file3.txt", "C2D2");      // CCDD

    RLEDecompressor decompressor;
    FakeOutputChannel output;

    SearchCommand command(storage, decompressor, output);

    std::vector<std::string> args = {"AA"};
    command.execute(args);

    vector<string> words = splitWords(output.getOutput());
    // We expect exactly file1.txt and file2.txt, in any order
    sort(words.begin(), words.end());
    vector<string> expected = {"file1.txt", "file2.txt"};

    EXPECT_EQ(expected, words);
}

// 3. No matching files
TEST(SearchCommandTest, PrintsNothingWhenNoFileMatches) {
    FakeStorage storage;

    storage.save("file1.txt", "A3B1");   // AAAB
    storage.save("file2.txt", "C2D2");   // CCDD

    RLEDecompressor decompressor;
    FakeOutputChannel output;

    SearchCommand command(storage, decompressor, output);

    std::vector<std::string> args = {"XYZ"};
    command.execute(args);

    vector<string> words = splitWords(output.getOutput());
    EXPECT_TRUE(words.empty());
}
