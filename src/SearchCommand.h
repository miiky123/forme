#ifndef SEARCH_COMMAND_H
#define SEARCH_COMMAND_H

#include "ICommand.h"
#include "IStorage.h"
#include "RLEDecompressor.h"
#include "IOutputChannel.h"  

#include <string>
#include <vector>

class SearchCommand : public ICommand {
public:
    SearchCommand(IStorage& storage,
                  RLEDecompressor& decompressor,
                  IOutputChannel& output);  

    void execute(const std::vector<std::string>& args) override;

private:
    IStorage& storage_;
    RLEDecompressor& decompressor_;
    IOutputChannel& output_;   
};

#endif
