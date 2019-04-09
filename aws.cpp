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

#define BUFF_SIZE 102400
#define SMALL_SIZE 100
#define QSIZE 10

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

void clientTcpServer() {
    // create a socket
    int serverSocket = socket(AF_INET, SOCK_STREAM, 0);
    if(serverSocket < 0) {
        cerr << "Can not creat a socket" << endl;
        return;
    }

    // configure server address
    int serverPort = atoi("24014");
    int serverIp = INADDR_ANY;
    sockaddr_in serverAddr;
    serverAddr.sin_addr.s_addr = htonl(serverIp);
    serverAddr.sin_port = htons(serverPort);
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
            cout << "received: " << string(receiveBuff, 0, BUFF_SIZE) << endl;
            writeToFile("test.txt", string(receiveBuff, 0, BUFF_SIZE));
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
    int serverPort = atoi("25014");
    int serverIp = INADDR_ANY;
    sockaddr_in serverAddr;
    serverAddr.sin_addr.s_addr = htonl(serverIp);
    serverAddr.sin_port = htons(serverPort);
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
            char receiveBuff[BUFF_SIZE];

            int receiveRes = recv(childSocket, receiveBuff, BUFF_SIZE, 0);
            cout << "received: " << string(receiveBuff, 0, BUFF_SIZE) << endl;
            string message = getLineFromFile("test.txt");
            if(!message.compare("empty")) {
                send(childSocket, message.c_str(), message.size() + 1, 0);
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
