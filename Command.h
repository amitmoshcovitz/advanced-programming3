#ifndef _COMMAND
#define _COMMAND

#include <sstream>
#include "DefaultIO.h"

class Command {
    private:
    std::string description;
    DefaultIO dio;
    public:
    virtual void execute() const;
    virtual std::string getDescription() const;
    virtual DefaultIO getDeafaultIO() const;
};

#endif