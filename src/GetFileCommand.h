#ifndef GET_FILE_COMMAND_H
#define GET_FILE_COMMAND_H

#include "ICommand.h"
#include "IStorage.h"
#include "RLEDecompressor.h"
#include "IOutputChannel.h"

#include <string>

class GetFileCommand : public ICommand {
    
public:
    GetFileCommand(IStorage& storage,
                   RLEDecompressor& decompressor,
                   IOutputChannel&  output);

    void execute(const std::vector<std::string>&) override;

private:
    IStorage& storage_;
    RLEDecompressor& decompressor_;
    IOutputChannel& output_;
};

#endif 

