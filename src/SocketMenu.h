#ifndef SOCKET_MENU_H
#define SOCKET_MENU_H

#include "IMenu.h"
#include "IConnection.h"

class SocketMenu : public IMenu {
public:
    explicit SocketMenu(IConnection& connection);

    bool nextCommand(std::string& line) override;

private:
    IConnection& connection_;
};

#endif // SOCKET_MENU_H
