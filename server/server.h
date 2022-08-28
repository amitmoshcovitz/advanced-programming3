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
#include <queue>

class Server {
    private:
    int sock;
    queue<int> clientSocks;

    public:
    /**
     * Construct a new Server object
     * @param port the port to listen to
     */
    Server(int port);

    /**
     * Runs the server.
     */
    void run();
    class ServerThread {
        private:
        map<Point, string> points;
        int k;
        Point::DistanceMetric metric;
        Server* server;

        public:
        /**
         * Constructor for the server thread class.
         */
        ServerThread(Server* server);

        /**
         * Destructor for the server class.
         */
        ~ServerThread();

        /**
         * Runs the server.
         */
        void run();

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
};