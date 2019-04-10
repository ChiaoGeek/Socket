#include <sys/types.h>
#include <sys/socket.h>

#include <iostream>
#include <arpa/inet.h>
#include <unistd.h>

#include <string>
#include <netinet/in.h>
#include <algorithm>
#include <netdb.h>
#include <cstring>
#include <sstream>
#include <fstream>
#include <ostream>

#include <vector>




#define BUFF_SIZE 102400
#define SMALL_SIZE 100
#define QSIZE 10

#define CLIENT_MONITOR_FILE ".client_monitor.txt"
#define CLINET_SERVER_PORT 24014
#define MONITOR_SERVER_PORT 25014

#define SERVERA_PORT     21014

using namespace std;

void writeToFile(string filename, string content) {
    ofstream myfile;
    myfile.open(filename.c_str());
    myfile << content;
    myfile.close();
}

bool isEmpty(string filename){
    ifstream file;
    file.open(filename.c_str());
    bool res = file.peek() == ifstream::traits_type::eof();
    file.close();
    return res;
}

string getLineFromFile(string filename) {
    string res;
    ifstream file;
    file.open(filename.c_str());
    if(isEmpty(filename)) {
        res = "empty";
    }else {
        getline(file, res);
    }
    file.close();
    return res;
}

void clearFile(string filename) {
    ofstream myfile;
    myfile.open(filename.c_str(), std::ofstream::out | std::ofstream::trunc);
    myfile.close();
}

vector<string> stringToVector(string s) {
    stringstream ss(s);
    string b;
    vector<string> v;
    while(ss >> b) {
        v.push_back(b);
    }
    return v;
}

void udpClient(string s) {

    // Creating socket file descriptor

    int udpClientSocket = socket(AF_INET, SOCK_DGRAM, 0);

    if ( udpClientSocket < 0 ) {
        cerr << "Can not create socket" << endl;
        return;
    }

    sockaddr_in  servaddr;
    servaddr.sin_family = AF_INET;
    servaddr.sin_port = htons(SERVERA_PORT);
    servaddr.sin_addr.s_addr = INADDR_ANY;
    socklen_t serverSize = sizeof(servaddr);

    sendto(udpClientSocket, s.c_str(), s.size(), 0, (const struct sockaddr *) &servaddr, sizeof(servaddr));

}

void clientTcpServer() {
    // create a socket
    int serverSocket = socket(AF_INET, SOCK_STREAM, 0);
    if(serverSocket < 0) {
        cerr << "Can not creat a socket" << endl;
        return;
    }

    // configure server address
    int serverIp = INADDR_ANY;
    sockaddr_in serverAddr;
    serverAddr.sin_addr.s_addr = htonl(serverIp);
    serverAddr.sin_port = htons(CLINET_SERVER_PORT);
    serverAddr.sin_family = AF_INET;
    socklen_t addrLen = sizeof(serverAddr);

    // bind a name to socket
    int bindResult = bind(serverSocket, (const sockaddr*) &serverAddr, addrLen);
    if(bindResult < 0) {
        cerr << "Can not bind the name to the socket" << endl;
        return;
    }

    // make a socket for listing
    int listening = listen(serverSocket, SOMAXCONN);
    if(listening < -1) {
        cerr << "Can not listen the socket" << endl;
        return;
    }

    // run the server
    while(true) {
        // define client address
        sockaddr_in clientAddr;
        socklen_t  clientSize;

        // create a child socket
        int childSocket = accept(serverSocket, (sockaddr*) &clientAddr, &clientSize);

        // create a child process
        int pid = fork();
        if(pid == 0) {
            if(childSocket > 0) {
                cout << "Client connected" << endl;
            }
            // receive message
            char receiveBuff[BUFF_SIZE];
            // send message
            char sendBuff[BUFF_SIZE];

            int receiveRes = recv(childSocket, receiveBuff, BUFF_SIZE, 0);
            string resMessage = string(receiveBuff, 0, BUFF_SIZE);
            vector<string> v = stringToVector(resMessage);
            string firstCommand = *(v.begin());
            cout << "received: " << resMessage << endl;
            // for monitor
            writeToFile(CLIENT_MONITOR_FILE, resMessage);
            if(firstCommand.compare("write") == 0) {
                udpClient(resMessage);
            }else if(firstCommand.compare("search") == 0) {
                udpClient(resMessage);
            }else if(firstCommand.compare("compute") == 0) {
                udpClient("search " + *(++v.begin()));
            }

            udpClient(resMessage);
            send(childSocket, receiveBuff, BUFF_SIZE + 1, 0);
            close(childSocket);
        }

    }
}

void monitorTcpSocket() {
    // create a socket
    int serverSocket = socket(AF_INET, SOCK_STREAM, 0);
    if(serverSocket < 0) {
        cerr << "Can not creat a socket" << endl;
        return;
    }

    // configure server address
    int serverIp = INADDR_ANY;
    sockaddr_in serverAddr;
    serverAddr.sin_addr.s_addr = htonl(serverIp);
    serverAddr.sin_port = htons(MONITOR_SERVER_PORT);
    serverAddr.sin_family = AF_INET;
    socklen_t addrLen = sizeof(serverAddr);

    // bind a name to socket
    int bindResult = bind(serverSocket, (const sockaddr*) &serverAddr, addrLen);
    if(bindResult < 0) {
        cerr << "Can not bind the name to the socket" << endl;
        return;
    }

    // make a socket for listing
    int listening = listen(serverSocket, SOMAXCONN);
    if(listening < -1) {
        cerr << "Can not listen the socket" << endl;
        return;
    }

    // run the server
    while(true) {
        // define client address
        sockaddr_in clientAddr;
        socklen_t  clientSize;

        // create a child socket
        int childSocket = accept(serverSocket, (sockaddr*) &clientAddr, &clientSize);

        // create a child process
        int pid = fork();
        if(pid == 0) {
            if(childSocket > 0) {
                cout << "Monitor Client connected" << endl;
            }
            // receive message
            while(true) {
                char receiveBuff[BUFF_SIZE];
                int receiveRes = recv(childSocket, receiveBuff, BUFF_SIZE, 0);
                if (receiveRes == -1)
                {
                    break;
                }
                string message = getLineFromFile(CLIENT_MONITOR_FILE);
                if(message.compare("empty") != 0) {
                    clearFile(CLIENT_MONITOR_FILE);
                }
                send(childSocket, message.c_str(), message.size(), 0);
            }
            close(childSocket);
        }

    }
}

int main(int argc, char* argv[]) {
    int pid = fork();
    if(pid == 0) {
        monitorTcpSocket();
    }else {
        clientTcpServer();
    }
    return 0;
}
