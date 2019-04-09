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

#define BUFF_SIZE 102400
#define SMALL_SIZE 100
#define QSIZE 10

using namespace std;

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
            send(childSocket, receiveBuff, BUFF_SIZE + 1, 0);
            close(childSocket);
        }

    }


}

int main(int argc, char* argv[]) {

//    int serv_socket = socket(AF_INET, SOCK_STREAM, 0);
//
//    int port = atoi("24014");
//
//    sockaddr_in server_addr;
//    server_addr.sin_family = AF_INET;
//    server_addr.sin_port = htons(port);
//    server_addr.sin_addr.s_addr = htonl(INADDR_ANY);
//    socklen_t length_server = sizeof(server_addr);
//
//    bind(serv_socket, (sockaddr*)&server_addr, length_server);
//
//    listen(serv_socket, QSIZE);

    clientTcpServer();

//    while (true) {
//
//        sockaddr_in client_addr;
//        socklen_t clientSize;
//
//        int new_socket = accept(serv_socket, (sockaddr*)&client_addr, &clientSize);
//
//        int pid = fork();
//
//        if (pid == 0) {
//
//            if (new_socket > 0) {
//                cout << "Client connected " << endl;
//            }
//
//            char recv_buff[BUFF_SIZE];
//            char send_buff[BUFF_SIZE];
//
//            int bytesRecv = recv(new_socket, recv_buff, BUFF_SIZE, 0);
//            cout << "received: " << string(recv_buff, 0, BUFF_SIZE) << endl;
//            send(new_socket, recv_buff, BUFF_SIZE + 1, 0);
//            close(new_socket);
////            break; // break the while loop in child
//        }
//
//    }
    return 0;

}
