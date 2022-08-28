#ifndef _CLI
#define _CLI
#include "../Command.h"

class CLI {
    private:
    const Command *commands[7] = {
        &UploadCommand(),
        &SettingsCommand(),
        &ClassifyCommand(),
        &DisplayResultsCommand(),
        &DownloadResultsCommand(),
        &DisplayConfusionMatrixCommand(),
        &ExitCommand()
    };
    const int NUM_OF_OPTIONS = 7;
    void printMenu();
    int getUserDecision();
    public:
    void start();
};
#endif