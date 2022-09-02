#include <string>
#include <fstream>
#include "../SocketIO.h"
#include <thread>

#define ERROR -1
#define SUCCESS 0
#define END '\u0003'

using namespace std;

class Client {
    private:
    SocketIO socketIO;

    /**
     * Writes a line of text the to a file.
     * @param line The line.
     * @param outputStream The file.
     * @return Whether an error accured.
     */
    int writeLine(std::string line, std::ofstream outputStream);

    public:
    static const int DEFAULT_BUFFER_SIZE = 4096;
    /**
     * Sends a line of text to the socket.
     * @param filePath The path to the file.
     * @return Whether an error accured.
     */
    int sendFile(std::string filePath);
    /**
     * Writes the information from a socket to a file.
     * @param filePath The path to the file.
     * @param str The string to write.
     */
    void writeFile(std::string filePath, string str);

    /**
     * Sends a line of text to the socket.
     * @param line The line.
     * @return int Whether an error accured.
     */
    void sendLine(std::string line);

    /**
     * Gets an input from the socket.
     * @return The input.
     */
    string getInput() const;

    /**
     * Creates a new client.
     * @param socket The socket.
     */
    Client(int socket);
};