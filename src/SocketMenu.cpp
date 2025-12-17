#include "SocketMenu.h"

SocketMenu::SocketMenu(IConnection& connection)
    : connection_(connection) {}

bool SocketMenu::nextCommand(std::string& line) {
    return connection_.readLine(line);
}
