#include "../Command.h"

class ClassifyCommand : protected Command {
    public:
    virtual void execute() const;
    virtual std::string getDescription() const;
    virtual DefaultIO getDeafaultIO() const;
};