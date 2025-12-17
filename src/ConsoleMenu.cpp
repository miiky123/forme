#include "ConsoleMenu.h"
#include <string>

ConsoleMenu::ConsoleMenu(std::istream& input, std::ostream& output)
    : input_(input),
      output_(output)
{
}

bool ConsoleMenu::nextCommand(std::string& line) {
    if (!std::getline(input_, line)) {
        return false;  
    }
    return true;
}
