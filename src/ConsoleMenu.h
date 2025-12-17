#ifndef CONSOLE_MENU_H
#define CONSOLE_MENU_H

#include "IMenu.h"
#include <istream>
#include <ostream>
#include <string>

class ConsoleMenu : public IMenu {
public:
    ConsoleMenu(std::istream& input, std::ostream& output);

    bool nextCommand(std::string& line) override;

private:
    std::istream& input_;
    std::ostream& output_;
};

#endif 
