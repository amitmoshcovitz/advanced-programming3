#ifndef _COMMAND
#define _COMMAND

#include <sstream>
#include "DefaultIO.h"

using namespace std;

class Command {
    protected:
    DefaultIO* dio;
    string description;
    public:
    virtual void execute() const = 0;
    string getDescription() const;
};

#endif