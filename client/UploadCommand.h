#include "../Command.h"

class UploadCommand : public Command {
    private:
    int sendFile(std::string filePath) const;
    public:
    virtual void execute() const;
    virtual std::string getDescription() const;
    virtual DefaultIO getDeafaultIO() const;
};