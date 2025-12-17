#ifndef IMENU_H
#define IMENU_H

#include <string>

class IMenu {
public:
    virtual ~IMenu() = default;

    // Read a full input line from the user.
    virtual bool nextCommand(std::string& line) = 0;
};

#endif // IMENU_H
