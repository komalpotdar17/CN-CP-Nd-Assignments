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

    SOCKET server_socket;
    struct sockaddr_in server_addr;
    int addr_len = sizeof(server_addr);

    server_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (server_socket == INVALID_SOCKET)
    {
        perror("Socket creation failed");
        exit(1);
    }

    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(PORT);
    server_addr.sin_addr.s_addr = INADDR_ANY;

    if (bind(server_socket, (struct sockaddr *)&server_addr, sizeof(server_addr)) == SOCKET_ERROR)
    {
        perror("Binding failed");
        exit(1);
    }

    if (listen(server_socket, 10) == SOCKET_ERROR)
    {
        perror("Listening failed");
        exit(1);
    }

    SOCKET client_socket;
    char buffer[BUFSIZE];
    FILE *fp;

    client_socket = accept(server_socket, (struct sockaddr *)&server_addr, &addr_len);
    if (client_socket == INVALID_SOCKET)
    {
        perror("Accepting connection failed");
        exit(1);
    }

    // Use fopen instead of fopen_s
    fp = fopen("hello.txt", "rb");
    if (fp == NULL)
    {
        perror("File opening failed");
        exit(1);
    }

    while (1)
    {
        size_t bytesRead = fread(buffer, 1, BUFSIZE, fp);
        if (bytesRead > 0)
        {
            send(client_socket, buffer, bytesRead, 0);
        }
        if (bytesRead < BUFSIZE)
        {
            if (feof(fp))
            {
                printf("File sent successfully\n");
            }
            break;
        }
    }

    // Wait for acknowledgment from client
    char ack[BUFSIZE];
    int ack_len = recv(client_socket, ack, BUFSIZE, 0);
    if (ack_len > 0)
    {
        ack[ack_len] = '\0';  // Null-terminate the acknowledgment message
        printf("Acknowledgment received from client: %s\n", ack);
    }
    else
    {
        printf("No acknowledgment received.\n");
    }

    closesocket(client_socket);
    closesocket(server_socket);
    fclose(fp);

    WSACleanup();
    return 0;
}
