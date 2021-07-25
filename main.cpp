#include <iostream>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <stdlib.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <string.h>
#include "help.h"
#define PORT 2040

using namespace std;

void init(int&, int&, struct sockaddr_in&, const char*);
int main(int argc, char* argv[])
{
    int socket_fd, connection;
    const int port = 2040;
    int buffersize;
    //char buffer[buffersize];
    struct sockaddr_in server_address;
    const char *ipaddress = "127.0.0.1";

    //init
    init(socket_fd, connection, server_address, ipaddress);

    //get arguments
    string args;
    for(int i = 1; i < argc; i++)
    {
        args += argv[i];
        args += ' ';
    }

    //string to buffer[]
    int length = args.length();
    char request[length +1];
    for(int i = 0; i < length; i++)
    {
        request[i] = args[i];
    }
    buffersize = sizeof(request);

    //check if user needs help
    if(strcmp(argv[1], "help") == 0)
    {
        cout << help_text << endl;
        exit(0);
    }

    //send args
    send(socket_fd, request, buffersize, 0);
    char response[1024];
    buffersize = sizeof(response);
    recv(socket_fd, response, buffersize, 0);
    cout << response << endl;
}
void init(int& socket_fd, int& connection, struct sockaddr_in& server_address, const char* ipaddress)
{
    socket_fd = socket(AF_INET, SOCK_STREAM, 0);
    if(socket_fd < 0)
    {
        cout << "\nFailed to create socket\n" << endl;
        exit(-1);
    }

    server_address.sin_family = AF_INET;
    server_address.sin_port = htons(PORT);
    int set_address = inet_pton(AF_INET, ipaddress, &server_address.sin_addr);
    if(set_address <= 0)
    {
        cout << "\nInvalid address\n" << endl;
        exit(-2);
    }

    connection = connect(socket_fd, (struct sockaddr *)&server_address, sizeof(server_address));
    if(connection < 0)
    {
        cout << "\nConnection failed\n"<< endl;
        exit(-3);
    }
}
