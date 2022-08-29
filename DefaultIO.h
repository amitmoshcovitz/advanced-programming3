#ifndef _DEFAULT_IO
#define _DEFAULT_IO
#include <string>

class DefaultIO {
    public:
    virtual void write(std::string str) const = 0;
    virtual std::string read() const = 0;
};
#endif