#include "FileStorage.h"
#include <fstream>
#include <stdexcept>
#include <filesystem>
#include <vector>
#include <mutex>

using std::string;

// Constructor: Initializes the storage and ensures the base directory exists
FileStorage::FileStorage(const string& baseDirectory)
: baseDirectory(baseDirectory)
{
// Validate if the base directory path is provided
if (this->baseDirectory.empty()) {
        throw std::invalid_argument("500 Internal Server Error");
}

std::filesystem::path dir(this->baseDirectory);
std::error_code ec;

// Check if directory exists; if not, attempt to create it
if (!std::filesystem::exists(dir, ec)) {
    if (!std::filesystem::create_directories(dir, ec)) {
        throw std::runtime_error(
            "500 Internal Server Error");
    }
// If path exists but isn't a directory, throw an error
} else if (!std::filesystem::is_directory(dir, ec)) {
    throw std::runtime_error(
        "500 Internal Server Error");
}
}

string FileStorage::buildFullPath(const string& fileName) const {
// Ensure a non-empty file name is provided
if (fileName.empty()) {
    throw std::invalid_argument("500 Internal Server Error");
}

// Handle cases where the base directory already ends with a separator
if (!baseDirectory.empty() &&
    (baseDirectory.back() == '/' || baseDirectory.back() == '\\')) {
        return baseDirectory + fileName;
}

return baseDirectory + "/" + fileName;
}

// Loads the full binary content of the specified file
std::string FileStorage::load(const string& fileName) const {
const string fullPath = buildFullPath(fileName);

// Attempt to open the file for binary reading
std::ifstream in(fullPath, std::ios::in | std::ios::binary);
if (!in) {
    throw std::runtime_error("500 Internal Server Error");
}

string result;

// Get file size and resize string buffer
in.seekg(0, std::ios::end);
std::streampos size = in.tellg();

if (size > 0) {
    result.resize(static_cast<std::size_t>(size));
        in.seekg(0, std::ios::beg);
            // Read all content into the resized string buffer
            in.read(&result[0], static_cast<std::streamsize>(size));

}

// Check for read errors unless End-Of-File was reached after reading
if (!in && !in.eof()) {
    throw std::runtime_error("500 Internal Server Error");
}

return result;
}

// Saves content to the specified file, overwriting existing data
void FileStorage::save(const string& fileName, const string& content) {
    // Acquire lock for thread safety; automatically released on exit
    std::lock_guard<std::mutex> lock(file_mutex); 
    
const string fullPath = buildFullPath(fileName);

// Attempt to open file for binary writing, truncating existing content
std::ofstream out(fullPath,
            std::ios::out | std::ios::binary | std::ios::trunc);
if (!out) {
    throw std::runtime_error("500 Internal Server Error");
}

// Write the entire content buffer to the file
out.write(content.data(), static_cast<std::streamsize>(content.size()));

// Check for write errors
if (!out) {
    throw std::runtime_error("500 Internal Server Error");
}
}

// Lists the file names present in the base directory
std::vector<std::string> FileStorage::listFiles() const {
std::vector<std::string> result;

std::filesystem::path dir(baseDirectory);
std::error_code ec;

// If the directory is invalid or missing, return an empty list immediately
if (!std::filesystem::exists(dir, ec) ||
    !std::filesystem::is_directory(dir, ec)) {
        return result;
}

// Iterate over directory entries to find regular files
for (const auto& entry : std::filesystem::directory_iterator(dir, ec)) {
    if (ec) {
        throw std::runtime_error(
            "500 Internal Server Error");
    }

    if (entry.is_regular_file(ec)) {
        // Collect only the file's name, not the full path
        result.push_back(entry.path().filename().string());
    }
}

if (ec) {
    throw std::runtime_error(
        "500 Internal Server Error");
}

return result; 
}

// Removes the file with the specified name from the storage
void FileStorage::remove(const std::string& file_name) {
    // Acquire lock for thread safety; automatically released on exit
    std::lock_guard<std::mutex> lock(file_mutex);
    
const string fullPath = buildFullPath(file_name);

std::filesystem::path p(fullPath);
std::error_code ec;

// First check: throw 'file not found' immediately if it doesn't exist
if (!std::filesystem::exists(p, ec)) {
    throw std::runtime_error("404 file not found");
}

// Attempt to remove the file.
if (!std::filesystem::remove(p, ec)) {
    // If removal fails for other reasons, we maintain 
    throw std::runtime_error("500 Internal Server Error");
}
}