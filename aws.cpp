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

//void clientTcpServer() {
//    int serv_socket = socket(AF_INEF, SOCK_STREAM, 0);
//
//}

int main(int argc, char* argv[]) {

    int serv_socket = socket(AF_INET, SOCK_STREAM, 0);

    int port = atoi("24014");

    sockaddr_in server_addr;
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(port);
    server_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    socklen_t length_server = sizeof(server_addr);

    bind(serv_socket, (sockaddr*)&server_addr, length_server);

    listen(serv_socket, QSIZE);

    while (true) {

        sockaddr_in client_addr;
        socklen_t clientSize;

        int new_socket = accept(serv_socket, (sockaddr*)&client_addr, &clientSize);

        int pid = fork();

        if (pid == 0) {

            if (new_socket > 0) {
                cout << "Client connected " << endl;
            }

            char recv_buff[BUFF_SIZE];
            char send_buff[BUFF_SIZE];

            int bytesRecv = recv(new_socket, recv_buff, BUFF_SIZE, 0);
            cout << "received: " << string(recv_buff, 0, BUFF_SIZE) << endl;
            send(new_socket, recv_buff, BUFF_SIZE + 1, 0);
            close(new_socket);
            break; // break the while loop in child
        }
        else {
//            close(new_socket);
            ;
        }

    }
    return 0;

}
