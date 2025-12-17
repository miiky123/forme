#include "App.h"
#include "IMenu.h"
#include "ICommand.h"
#include "IOutputChannel.h"  

#include <string>
#include <vector>
#include <map>
#include <cstddef>
#include <cctype>             

App::App(IMenu* menu,
         IOutputChannel& output,
         const std::map<std::string, ICommand*>& commands)
    : menu_(menu),
      output_(output),
      commands_(commands)
{
}

static std::size_t skipSpaces(const std::string& s, std::size_t i) {
    while (i < s.size() && s[i] == ' ') {
        ++i;
    }
    return i;
}

void App::run() {
    std::string line;

    while (true) {
        // Read one full line from the menu
        if (!menu_->nextCommand(line)) {
            // End of input
            continue;
        }

        if (line.empty()) {
            continue;
        }

        // Split line into commandName and the rest (rawArgs)
        std::size_t firstSpace = line.find(' ');
        std::string commandName;
        std::string rawArgs;

        if (firstSpace == std::string::npos) {
            commandName = line;
        } else {
            commandName = line.substr(0, firstSpace);
            if (firstSpace + 1 < line.size()) {
                rawArgs = line.substr(firstSpace + 1);
            }
        }

        if (commandName.empty()) {
            continue;
        }

        // normalize commandName to lowercase
        for (char& c : commandName) {
            c = static_cast<char>(std::tolower(static_cast<unsigned char>(c)));
        }

        auto it = commands_.find(commandName);
        if (it == commands_.end()) {
            // Unknown command: respond with error TO THE OUTPUT CHANNEL
            output_.write("400 Bad Request\n");
            continue;
        }

        // Generic splitting of rawArgs by spaces.
        // Concrete commands are responsible for interpreting this vector.
        std::vector<std::string> args;
        std::size_t i = skipSpaces(rawArgs, 0);
        while (i < rawArgs.size()) {
            std::size_t start = i;
            while (i < rawArgs.size() && rawArgs[i] != ' ') {
                ++i;
            }
            args.push_back(rawArgs.substr(start, i - start));
            i = skipSpaces(rawArgs, i);
        }

        // Execute command, but don't crash the program on bad args
        try {
            it->second->execute(args);
        } catch (...) {
            // Again, report error via the output channel (socket / console)
        }
    }
}