#include <string>
#include <fstream>

#define ERROR -1
#define SUCCESS 0
#define END '\u0003'

class Client {
    private:
    /**
     * Sends a line of text to the socket.
     * @param line The line.
     * @param socket The socket.
     * @return Whether an error accured.
     */
    int sendLine(std::string line, int socket);
    /**
     * Gets a line of text from the socket.
    * @param socket The socket.
     * @param buffer The buffer to load the information.
     * @param bufferSize The size of the buffer.
     * @return Whether an error accured.
     */
    int getLine(int socket, char* buffer, int bufferSize);
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
     * @param socket The socket.
     * @return Whether an error accured.
     */
    int sendFile(std::string filePath, int socket);
    /**
     * Writes the information from a socket to a file.
     * @param socket The socket.
     * @param filePath The path to the file.
     * @return Whether an error accured.
     */
    int writeFileFromSocket(int socket, std::string filePath);
};