#ifndef IOUTPUTCHANNEL_H
#define IOUTPUTCHANNEL_H

#include <string>

class IOutputChannel {
public:
    virtual ~IOutputChannel() = default;

    // Write data to the output (may contain '\n')
    virtual void write(const std::string& data) = 0;
};

#endif // IOUTPUTCHANNEL_H
