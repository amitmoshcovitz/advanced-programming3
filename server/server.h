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

    private:
    /**
     * Adds a client to the queue of clients.
     * @return if timed out
     */
    bool acceptClient();
    class ServerThread {
        private:
        const int defaultK = 5;
        const Point::DistanceMetric defaultMetric = Point::DistanceMetric::EUCLIDEAN;
        map<Point, string> points;
        int k;
        Point::DistanceMetric metric;
        Server* server;
        vector<Point> testPoints;
        vector<string> testResults;
        bool isStopped;
        bool running;

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
         * Creates and runs the thread.
         */
        void start();

        /**
         * Stops the thread.
         */
        void stop();

        /**
         * Returns if the thread is running.
         * @return if the thread is running
         */
        bool isRunning();

        private:
        /**
         * Accepts a connection from a client.
         */
        int acceptClient() const;

        /**
         * Send a message to a client.
         * @param clientSock The socket to send the message to.
         * @param message The message to send.
         */
        void sendToClient(int clientSock, string message) const;

        /**
         * Receive a message from a client.
         * @param clientSock The socket to receive the message from.
         * @return The message.
         */
        string receiveFromClient(int clientSock) const;

        /**
         * Detects if a point is in the right dimension.
         * @param point The point to check.
         * @return Whether the point is in the right dimension.
         */
        bool isValid(const Point &point) const;

        /**
         * The task for the thread.
         */
        void run();

        /**
         * Resets the data from the previous client.
         */
        void reset();

        /**
         * Handles a client.
         */
        void handleClient(int clientSock);

        /**
         * Handles a client's train file upload.
         * @param file the entire text of the file
         * @return is the file valid
         */
        bool handleTrainUpload(string file);

        /**
         * Handles a client's test upload.
         * @param file the entire text of the file
         * @return is the file valid
         */
        bool handleTestUpload(string file);

        /**
         * Processes a client's test file.
         */
        void processTest();
    };
};