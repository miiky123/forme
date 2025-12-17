#ifndef DELETECOMMAND_H
#define DELETECOMMAND_H

#include "ICommand.h"
#include "IStorage.h"
#include "IOutputChannel.h"

#include <string>
#include <vector>

class DeleteCommand : public ICommand {
public:

DeleteCommand(IStorage& storage, IOutputChannel& output);

// Checks argument count, attempts deletion. Handles the IStorage exceptions 
// and sends either "204 No Content" (Success) or "404 Not Found" (File Missing) 
// to the output channel
void execute(const std::vector<std::string>& args) override;

private:
// Reference to the storage system dependency
IStorage& storage_;
// Reference to the output channel dependency for sending status codes
IOutputChannel& output_; 
};

#endif