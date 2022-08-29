#ifndef _STANDARD_IO
#define _STANDARD_IO
#include "DefaultIO.h"

class StandardIO : public DefaultIO {
    public:
    void write(std::string) const;
    std::string read() const;
};
#endif