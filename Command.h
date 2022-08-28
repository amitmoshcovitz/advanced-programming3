#ifndef _COMMAND
#define _COMMAND

#include <sstream>
#include "DefaultIO.h"

class Command {
    private:
    std::string description;
    DefaultIO dio;
    public:
    virtual void execute();
    virtual std::string getDescription();
    virtual DefaultIO getDeafaultIO();
};

#endif