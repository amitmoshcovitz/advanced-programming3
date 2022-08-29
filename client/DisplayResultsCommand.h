#include "../Command.h"

class DisplayResultsCommand : protected Command {
    public:
    virtual void execute() const;
    virtual std::string getDescription() const;
    virtual DefaultIO getDeafaultIO() const;
};