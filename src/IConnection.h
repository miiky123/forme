#ifndef ICONNECTION_H
#define ICONNECTION_H

#include <string>

class IConnection {
public:
    virtual ~IConnection() = default;

    // Read a line 
    // Return false if the peer closed the connection or on fatal error.
    virtual bool readLine(std::string& line) = 0;

    // Write raw data to the connection.
    virtual void write(const std::string& data) = 0;
};

#endif // ICONNECTION_H
