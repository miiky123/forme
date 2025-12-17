#include <cassert>
#include <iostream>
#include <map>
#include <string>
#include <vector>
#include <stdexcept>


#include "App.h"
#include "IMenu.h"
#include "ICommand.h"
#include "IStorage.h"
#include "IOutputChannel.h"
#include "AddCommand.h"
#include "SearchCommand.h"
#include "RLECompressor.h"
#include "RLEDecompressor.h"



class FakeStorage : public IStorage {
public:
    void save(const std::string& fileName,
              const std::string& content) override {
        data_[fileName] = content;
    }

    std::string load(const std::string& fileName) const override {
        auto it = data_.find(fileName);
        if (it == data_.end()) {
            throw std::runtime_error("not found");
        }
        return it->second;
    }

    std::vector<std::string> listFiles() const override {
        std::vector<std::string> files;
        for (const auto& kv : data_) {
            files.push_back(kv.first);
        }
        return files;
    }

    // implement remove so FakeStorage is not abstract
    void remove(const std::string& fileName) override {
        auto it = data_.find(fileName);
        if (it == data_.end()) {
            throw std::runtime_error("not found");
        }
        data_.erase(it);
    }

private:
    std::map<std::string, std::string> data_;
};

// Collects everything written by commands
class FakeOutputChannel : public IOutputChannel {
public:
    void write(const std::string& data) override {
        buffer_ += data;
    }

    const std::string& buffer() const { return buffer_; }
    void clear() { buffer_.clear(); }

private:
    std::string buffer_;
};


class FakeMenu : public IMenu {
public:
    explicit FakeMenu(const std::vector<std::string>& lines)
        : lines_(lines), index_(0) {}

    bool nextCommand(std::string& line) override {
        if (index_ >= lines_.size()) {
            return false;
        }
        line = lines_[index_++];
        return true;
    }

private:
    std::vector<std::string> lines_;
    std::size_t index_;
};


// POST happy path: should store file and return 201
void test_post_success() {
    FakeStorage storage;
    RLECompressor compressor;
    FakeOutputChannel out;

    AddCommand post(storage, compressor, out);

    std::vector<std::string> args = {"file1", "hello", "world"};
    post.execute(args);

    // Status line
    assert(out.buffer() == "201 Created\n");

    // File must be stored
    auto files = storage.listFiles();
    assert(files.size() == 1);
    assert(files[0] == "file1");
}

// POST with missing args: should return 400
void test_post_missing_args() {
    FakeStorage storage;
    RLECompressor compressor;
    FakeOutputChannel out;

    AddCommand post(storage, compressor, out);

    std::vector<std::string> args;  // empty
    post.execute(args);

    assert(out.buffer() == "400 Bad Request\n");
}

// SEARCH: should return 200 and find by name + content
void test_search_by_name_and_content() {
    FakeStorage storage;
    RLECompressor compressor;
    RLEDecompressor decompressor;
    FakeOutputChannel out;

    // Prepare two files 
    std::string compressed1 = compressor.compress("hello great world");
    std::string compressed2 = compressor.compress("some other text");

    storage.save("great_report", compressed1);  // term in name + content
    storage.save("notes", compressed2);

    SearchCommand searchCmd(storage, decompressor, out);

    std::vector<std::string> args = {"great"};
    searchCmd.execute(args);

    std::string outStr = out.buffer();

    //  start with status line + blank line
    const std::string prefix = "200 Ok\n\n";
    assert(outStr.rfind(prefix, 0) == 0);  // starts_with(prefix)

    std::string body = outStr.substr(prefix.size());

    // Body should at contain the file whose name matches
    assert(body.find("great_report") != std::string::npos);
}

int main() {
    test_post_success();
    test_post_missing_args();
    test_search_by_name_and_content();

    std::cout << "All server tests passed.\n";
    return 0;
}
