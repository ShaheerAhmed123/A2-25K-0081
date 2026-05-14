#ifndef COMMUNICATOR_H
#define COMMUNICATOR_H
#include <string>

// Interface for messaging between users
class Communicator {
public:
    virtual void send(std::string m) = 0;
    virtual ~Communicator() {}
};

#endif