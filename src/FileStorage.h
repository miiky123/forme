#ifndef FILE_STORAGE_H
#define FILE_STORAGE_H

#include "IStorage.h"
#include <string>
#include <vector>
#include <mutex>

class FileStorage : public IStorage {
public:
    explicit FileStorage(const std::string& baseDirectory);

    std::string load(const std::string& fileName) const override;
    void save(const std::string& fileName,
              const std::string& content) override;
    std::vector<std::string> listFiles() const override;
    void remove(const std::string& file_name) override;

private:
    std::string baseDirectory;

    std::string buildFullPath(const std::string& fileName) const;
    std::mutex file_mutex;

};

#endif
