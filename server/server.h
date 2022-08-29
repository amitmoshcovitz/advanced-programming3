#ifndef SERVER_H
#define SERVER_H
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
#include "../DefaultIO.h"
#include "../Command.h"
#include "../SocketIO.h"
#include "../StandardIO.h"
#include <string>
#include <thread>

using namespace std;

#define IO 1

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
        #if IO == 0
        SocketIO dio;
        #else
        StandardIO dio;
        #endif
        vector<Command*> commands;
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

        /**
         * The task for the thread.
         */
        void run();

        private:
        /**
         * Accepts a connection from a client.
         */
        int acceptClient() const;

        /**
         * Detects if a point is in the right dimension.
         * @param point The point to check.
         * @return Whether the point is in the right dimension.
         */
        bool isValid(const Point &point) const;

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

        class UploadCommand : public Command {
            private:
            ServerThread* serverThread;
            public:
            UploadCommand(DefaultIO* dio, ServerThread* serverThread);
            void execute() const;
        };

        class AlgorithmSettingsCommand : public Command {
            private:
            ServerThread* serverThread;
            public:
            AlgorithmSettingsCommand(DefaultIO* dio, ServerThread* serverThread);
            void execute() const;
        };

        class DetectCommand : public Command {
            private:
            ServerThread* serverThread;
            public:
            DetectCommand(DefaultIO* dio, ServerThread* serverThread);
            void execute() const;
        };

        class DisplayCommand : public Command {
            private:
            ServerThread* serverThread;
            public:
            DisplayCommand(DefaultIO* dio, ServerThread* serverThread);
            void execute() const;
        };

        class DownloadCommand : public Command {
            private:
            ServerThread* serverThread;
            public:
            DownloadCommand(DefaultIO* dio, ServerThread* serverThread);
            void execute() const;
        };

        class AnalyzeCommand : public Command {
            private:
            ServerThread* serverThread;
            public:
            AnalyzeCommand(DefaultIO* dio, ServerThread* serverThread);
            void execute() const;
        };

        class ExitCommand : public Command {
            private:
            ServerThread* serverThread;
            public:
            ExitCommand(DefaultIO* dio, ServerThread* serverThread);
            void execute() const;
        };
    };
};
#endif