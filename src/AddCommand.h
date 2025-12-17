#ifndef ADD_COMMAND_H
#define ADD_COMMAND_H

#include "ICommand.h"
#include "IStorage.h"
#include "RLECompressor.h"
#include "IOutputChannel.h" 
#include <string>
#include <vector>

class AddCommand : public ICommand {
public:
    AddCommand(IStorage& storage,
               RLECompressor& compressor,
               IOutputChannel& out
            );

    void execute(const std::vector<std::string>& args) override;

private:
    IStorage& storage_;
    RLECompressor& compressor_;
    IOutputChannel& out_;
};

#endif // ADD_COMMAND_H
