#ifdef WIN32
#include <windows.h>
#else
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#endif
#include <map>
#include "../point.h"
#include <fstream>

class Server {
    private:
    int sock;
    map<Point, string> points;
    int k;
    Point::DistanceMetric metric;

    public:
    /**
     * Constructor for the server class.
     * @param port The port to listen on.
     * @param k The number of nearest neighbors to return.
     * @param metric The distance metric to use.
     */
    Server(int port, int k, Point::DistanceMetric metric);

    /**
     * Destructor for the server class.
     */
    ~Server();

    /**
     * Returns the socket that the server is listening on.
     * @return The socket that the server is listening on.
     */
    int getSock() const;

    /**
     * Runs the server.
     */
    void run();

    /**
     * Loads the classified points from the file.
     * @param filename The name of the file to load from.
     */
    void loadClassified(string fileName);

    private:
    /**
     * Accepts a connection from a client.
     */
    int acceptClient() const;

    /**
     * Send a message to a client.
     * @param clientSock The socket to send the message to.
     * @param buffer The message to send.
     * @param bufferSize The length of the message.
     */
    void sendToClient(int clientSock, const char* buffer, int bufferSize) const;

    /**
     * Receive a message from a client.
     * @param clientSock The socket to receive the message from.
     * @param buffer The buffer to store the message in.
     * @param bufferSize The size of the buffer.
     * @return Whether the message is complete.
     */
    bool receiveFromClient(int clientSock, char* buffer, int bufferSize) const;

    /**
     * Detects if a point is in the right dimension.
     * @param point The point to check.
     * @return Whether the point is in the right dimension.
     */
    bool isValid(const Point &point) const;
};