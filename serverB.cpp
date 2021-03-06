#include <iostream>
#include <sys/types.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <string.h>
#include <string>
#include <sstream>
#include <fstream>
#include <ostream>

#include <stdio.h>
#include <stdlib.h>
#include <sstream>

#include <cmath>

#include <vector>

#define BUFF_SIZE 102400

#define SERVER_PORT 22014
#define SERVER_IP "0.0.0.0"

#define AWS_UDP_SERVER_PORT     23014

#define DATA_FILE "database.txt"
#define DATA_COUNT "count.txt"


using namespace std;

void writeToFile(string filename, string content) {
    ofstream myfile;
    myfile.open(filename.c_str());
    myfile << content;
    myfile.close();
}

void appendToFile(string filename, string content) {
    ofstream myfile;
    myfile.open(filename.c_str(), std::ios_base::app);
    myfile << content  << endl;
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

int getCurrentNum(string filename) {
    string res = getLineFromFile(filename);
    if(res.compare("empty") == 0) {
        return 0;
    }else {
        return atoi(res.c_str());
    }
}

int stringToInt(string str) {
    int num = 0;
    for(int i = 0; i < str.size(); i++) {
        num *= 10;
        num += (str[i] - '0');
    }
    return num;
}

string intToString(int num) {
    stringstream s;
    s << num;
    return s.str();
}

string doubleToString(double num) {
    stringstream s;
    s << num;
    return s.str();
}

double stringToDouble(string s) {
    return atof(s.c_str());
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

string calculate(string s) {
    vector<string> v = stringToVector(s);

    if(v.size() != 10) {
        return "Error";
    }
    double size = stringToDouble(v[2]);
    double signalsize = stringToDouble(v[3]);
    double bw = stringToDouble(v[6]);
    double length = stringToDouble(v[7]);
    double velocity = stringToDouble(v[8]);
    double noisepower = stringToDouble(v[9]);

    double capacity = (log(1 + pow(10, signalsize / 10 - 3) / pow(10, noisepower / 10 - 3)) / log(2)) * (bw * pow(10, 6));
    double t_f = pow(10,3) * (size / capacity);
    double t_p = (pow(10,3) * length) / velocity;
    double t_e = t_f + t_p;

    string res = "";
    res += doubleToString(t_f) + " ";
    res += doubleToString(t_p) + " ";
    res += doubleToString(t_e);
    return res;



}

string getStringFromVector(int i, vector<string> v) {
    string res;
    for(int j = i; j < v.size(); j++) {
        res += v[j] + " ";
    }
    return res;
}

string getLineFromFile(int id, string filename) {
    string res;
    ifstream file;
    file.open(filename.c_str());
    if(isEmpty(filename)) {
        res = "empty";
    }else {
        for(int i = 1; i <= id; i++) {
            getline(file, res);
        }
    }
    file.close();
    return res;
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
    servaddr.sin_port = htons(AWS_UDP_SERVER_PORT);
    servaddr.sin_addr.s_addr = INADDR_ANY;
    socklen_t serverSize = sizeof(servaddr);

    sendto(udpClientSocket, s.c_str(), s.size(), 0, (const struct sockaddr *) &servaddr, sizeof(servaddr));

}

void udpServer() {
    // create a socket
    int udpSocket = socket(AF_INET, SOCK_DGRAM, 0);
    if(udpSocket < 0) {
        cerr << "Can not create a udp server";
        return;
    }

    //bind the socket to a IP / port
    sockaddr_in hint;
    hint.sin_family = AF_INET;
    hint.sin_port = htons(SERVER_PORT);
    inet_pton(AF_INET, SERVER_IP, &hint.sin_addr);

    //bind the socket to a IP / port
    if(::bind(udpSocket, (sockaddr *)&hint, sizeof(hint)) < 0)
    {
        cerr << "Can't bind to IP/port";
        return;
    }

    sockaddr_in client;
    socklen_t clientSize = sizeof(client);

    char buf[BUFF_SIZE];     /* receive buffer */
    cout << "The Server B is up and running using UDP on port " + intToString(SERVER_PORT)+ "." << endl;
    for (;;) {
        int recvlen = recvfrom(udpSocket, buf, BUFF_SIZE, 0, (struct sockaddr *)&client, &clientSize);
        if (recvlen > 0) {

            string rMessage = string(buf, 0, recvlen);
            vector<string> v = stringToVector(rMessage);

            cout << "The Server B received link information: link "+ v[1] +", file size "+ v[2] +", and signal power "+ v[2] << endl;

            if(v.size() > 0) {
                string response = "res ";
                string firstStr = *(v.begin());
                int currNum = getCurrentNum(DATA_COUNT);
                if(firstStr.compare("compute") == 0) {
                    response += calculate(rMessage);
                }
                cout << "The Server B finished the calculation for link " << v[1] << endl;
                udpClient(response);
                cout << "The Server B finished sending the output to AWS" << endl;
            }
        }
    }
}

int main(int argc, char** argv) {
    udpServer();
}



