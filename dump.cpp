#include <iostream>
#include <thread>
#include <vector>
#include <map>

#ifdef _WIN32
// Windows-specific code
#include <winsock2.h>
#pragma comment(lib, "ws2_32.lib")
#else
// POSIX-specific code
#include <sys/socket.h>
#endif

/* create a map that stores the client's username and thread */
std::map<std::string, std::thread> clients;

/* handle client in thread */
void handle()
{

}

/* receive incoming clients */
void receive()
{

}

int main()
{
    // Initialize Winsock
    WSADATA wsaData;
    if (WSAStartup(MAKEWORD(2,2), &wsaData) != 0)
    {
        std::cout << "WSAStartup failed.\n";
        return 1;
    }

    SOCKET serverSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (serverSocket == INVALID_SOCKET)
    {
        std::cerr << "Failed to create socket.\n";
        WSACleanup();
        return -1;
    }

    sockaddr_in serverAddress;
    serverAddress.sin_family = AF_INET;
    serverAddress.sin_addr.s_addr = INADDR_ANY; // listen on any available netw interface
    serverAddress.sin_port = htons(8080); // listen on port 8080
    
    // bind socket to address

    // listen for incoming clients/connections

    // accept incoming clients




    return 0;
}