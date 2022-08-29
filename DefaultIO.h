#ifndef _DEFAULT_IO
#define _DEFAULT_IO
class DefaultIO {
    public:
    void write(std::string) const;
    std::string read() const;
};
#endif