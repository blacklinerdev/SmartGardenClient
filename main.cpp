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
#define Stream_Size 1024

using namespace std;

void init(int&, int&, struct sockaddr_in&, const char*);
string get_argstr(int&, char*[]);
void check_args(int&, char*[]);
char* get_request(string&, int&);

int main(int argc, char* argv[])
{
    //socket
    int socket_fd, connection;
    struct sockaddr_in server_address;
    const char *ipaddress = "127.0.0.1";
    //args
    string args = get_argstr(argc, argv);
    check_args(argc, argv);
    int requestsize = args.length() + 1;
    //init
    init(socket_fd, connection, server_address, ipaddress);

    //send
    const char* test = args.c_str();
    send(socket_fd, test, requestsize, 0);
    //recieve
    int responsesize = Stream_Size;
    char response[responsesize];
    recv(socket_fd, response, responsesize, 0);
    cout << response << endl;
}
string get_argstr(int& argc, char* argv[])
{
    //get arguments
    string args;
    for(int i = 1; i < argc; i++)
    {
        args += argv[i];
        args += ' ';
    }
    return args;
}
void check_args(int& argc, char* argv[])
{
    //check if args are empty
    if(argc <= 1)
    {
        cout << "\nInvalid arguments" << endl
            << help_text << endl;
        exit(0);
    }
    //check if user needs help
    if(strcmp(argv[1], "help") == 0)
    {
        cout << help_text << endl;
        exit(0);
    }
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
