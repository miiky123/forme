#ifndef APP_H
#define APP_H

#include <map>
#include <string>

class IMenu;
class ICommand;
class IOutputChannel;

class App {
public:
    App(IMenu* menu,
        IOutputChannel& output,
        const std::map<std::string, ICommand*>& commands);

    void run();

private:
    IMenu* menu_;
    IOutputChannel& output_;
    std::map<std::string, ICommand*> commands_;
};

#endif // APP_H
