#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <winsock2.h>

#define PORT 8080
#define BUFSIZE 1024

int main()
{
    WSADATA wsa;
    if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0)
    {
        perror("WSAStartup failed");
        exit(1);
    }

    SOCKET client_socket;
    struct sockaddr_in server_addr;
    FILE *fp;

    client_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (client_socket == INVALID_SOCKET)
    {
        perror("Socket creation failed");
        exit(1);
    }

    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(PORT);
    server_addr.sin_addr.s_addr = inet_addr("127.0.0.1");

    if (connect(client_socket, (struct sockaddr *)&server_addr, sizeof(server_addr)) == SOCKET_ERROR)
    {
        perror("Connection failed");
        exit(1);
    }

    // Open the file to receive
    fp = fopen("received_file.txt", "wb");
    if (fp == NULL)
    {
        perror("File opening failed");
        exit(1);
    }

    char buffer[BUFSIZE];
    size_t bytesRead;

    while (1)
    {
        bytesRead = recv(client_socket, buffer, BUFSIZE, 0);
        if (bytesRead <= 0)
        {
            break;
        }
        fwrite(buffer, 1, bytesRead, fp);
    }

    closesocket(client_socket);
    fclose(fp);

    WSACleanup();
    return 0;
}
