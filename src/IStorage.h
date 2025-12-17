#ifndef ISTORAGE_H
#define ISTORAGE_H

#include <string>
#include <vector>

class IStorage {
public:
    virtual ~IStorage() = default;

    // Save content under a logical file name
    virtual void save(const std::string& fileName,
                      const std::string& content) = 0;

    // Load content of a logical file name
    virtual std::string load(const std::string& fileName) const = 0;

    // List all logical file names stored in the system
    virtual std::vector<std::string> listFiles() const = 0;

    /**
     * @brief Removes/deletes a file with the given name. 
     * @throws std::runtime_error if the file is not found (404 Not Found).
     */
    virtual void remove(const std::string& file_name) = 0;
};

#endif