#include "../Command.h"

class SettingsCommand : protected Command {
    public:
    virtual void execute() const;
    virtual std::string getDescription() const;
    virtual DefaultIO getDeafaultIO() const;
};