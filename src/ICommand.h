#ifndef ICOMMAND_H
#define ICOMMAND_H

#include <vector>
#include <string>

class ICommand {
public:
    virtual ~ICommand() = default;

    // args holds all arguments for the command.
    virtual void execute(const std::vector<std::string>& args) = 0;
};

#endif // ICOMMAND_H
