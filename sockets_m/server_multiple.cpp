#include <winsock2.h>
#include <windows.h>
#include <ws2tcpip.h>
#include <stdlib.h>
#include <stdio.h>
#include <thread>
#include <iostream>
#include <cstring>

#define DEFAULT_BUFLEN 512

#define DEFAULT_PORT "8080"

void handle(SOCKET client)
{
    int iResult, iSendResult;
    char recvbuf[DEFAULT_BUFLEN];
    int recvbuflen = DEFAULT_BUFLEN;
    do
    {
        memset(recvbuf, 0, recvbuflen); // Clear the buffer for safety
        iResult = recv(client, recvbuf, recvbuflen, 0);
        if (iResult > 0)
        {
            int x, y, sum;
            std::string message(recvbuf);
            printf("Bytes received: %d\n", iResult);
            printf("Message received: %s\n", recvbuf);

            sscanf(message.c_str(),"%i %i",&x,&y);
            sum = x + y;

            // Convert the sum to a string to be sent back
            char sendBuf[DEFAULT_BUFLEN]; // Buffer to hold the sum
            sprintf(sendBuf, "%d", sum); // Convert the sum into a string

            // Echo the buffer back to the sender
            iSendResult = send(client, sendBuf, strlen(sendBuf), 0);
            if (iSendResult == SOCKET_ERROR)
            {
                printf("send failed with error: %d\n", WSAGetLastError());
                closesocket(client);
                WSACleanup();
                return;
            }
            printf("Bytes sent: %d\n", iSendResult);
        }

        else if (iResult == 0)

            printf("Connection closing...\n");

        else
        {
            printf("recv failed with error: %d\n", WSAGetLastError());
            closesocket(client);
            WSACleanup();
            return;
        }

    } while (iResult > 0);

    // close connection when done
    shutdown(client, SD_SEND);
    closesocket(client);
    WSACleanup();

}



int main()
{
    WSADATA wsaData;
    int iResult;

    SOCKET ListenSocket = INVALID_SOCKET;
    SOCKET ClientSocket = INVALID_SOCKET;

    struct addrinfo *result = NULL;
    struct addrinfo hints;

    int iSendResult;
    char recvbuf[DEFAULT_BUFLEN];
    int recvbuflen = DEFAULT_BUFLEN;

    // Initialize Winsock
    iResult = WSAStartup(MAKEWORD(2, 2), &wsaData);

    if (iResult != 0)
    {
        printf("WSAStartup failed with error: %d\n", iResult);
        return 1;
    }

    ZeroMemory(&hints, sizeof(hints));
    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_protocol = IPPROTO_TCP;
    hints.ai_flags = AI_PASSIVE;

    // Resolve the server address and port

    iResult = getaddrinfo(NULL, DEFAULT_PORT, &hints, &result);

    if (iResult != 0)
    {
        printf("getaddrinfo failed with error: %d\n", iResult);
        WSACleanup();
        return 1;
    }

    // Create a SOCKET for the server to listen for client connections.

    ListenSocket = socket(result->ai_family, result->ai_socktype, result->ai_protocol);

    if (ListenSocket == INVALID_SOCKET)
    {

        printf("socket failed with error: %ld\n", WSAGetLastError());
        freeaddrinfo(result);
        WSACleanup();
        return 1;
    }

    // Setup the TCP listening socket

    iResult = bind(ListenSocket, result->ai_addr, (int)result->ai_addrlen);

    if (iResult == SOCKET_ERROR)
    {

        printf("bind failed with error: %d\n", WSAGetLastError());
        freeaddrinfo(result);
        closesocket(ListenSocket);
        WSACleanup();
        return 1;
    }

    freeaddrinfo(result);

    // listen on socket
    iResult = listen(ListenSocket, SOMAXCONN);
    if (iResult == SOCKET_ERROR)
    {
        printf("listen failed with error: %d\n", WSAGetLastError());
        closesocket(ListenSocket);
        WSACleanup();
        return 1;
    }

    // Accept and handle incoming connections
    std::cout << "<listening for clients>" << std::endl;

    while (true)
    {
        ClientSocket = accept(ListenSocket, NULL, NULL);
        if (ClientSocket == INVALID_SOCKET)
        {
            printf("accept failed with error: %d\n", WSAGetLastError());
            closesocket(ListenSocket);
            WSACleanup();
            return 1;
        }

        // handle client in thread
        std::thread clientThread(handle, ClientSocket);
        clientThread.detach();
        std::cout << "<Client received>" << std::endl;

    }

    // Cleanup
    closesocket(ListenSocket);
    WSACleanup();

    return 0;
}