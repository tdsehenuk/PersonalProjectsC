#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <winsock2.h>


int main(int argc, char *argv[]) {

    WSADATA wsa;
    if (WSAStartup(MAKEWORD(2,2), &wsa) != 0) {
        printf("Failed to initialize Winsock. Error code: %d\n", WSAGetLastError());
        return 1;
    }

    //IPv4, TCP, system chooses protocol
    SOCKET server_fd = socket(AF_INET, SOCK_STREAM, 0);
    //socket() returns a unsigned int so if its value is -1 it failed
    if (server_fd == INVALID_SOCKET) {
        perror("socket failed");
        exit(EXIT_FAILURE);
    }

    struct sockaddr_in server_addr; 
    //need to fill the structure members family, ip addr and port no
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY; //Listen on any interface + sin_addr has nested struct s_addr
    server_addr.sin_port =htons(8080);  //port 8080, htons converts to network byte order

    //bind takes a socket file descriptor, pointer to a struct of ip/port to bind socket with, and number of bytes of addr struct
    if(bind(server_fd, (struct sockaddr*)&server_addr, sizeof(server_addr)) == SOCKET_ERROR) {
        printf("Bind failed with error code: %d\n", WSAGetLastError());
        closesocket(server_fd);
        WSACleanup();
        return 1;
    }

    printf("Bind successful. Listening on port 8080...\n");

    //int listen(SOCKET sockfd, int backlog)
    //SOMAXCONN = backlog to maximum reasonal value
    if(listen(server_fd, SOMAXCONN) == SOCKET_ERROR) {
        printf("Listen failed with error code: %d\n", WSAGetLastError());
        closesocket(server_fd);
        WSACleanup();
        return 1;
    }

    printf("Listening for incoming connections...\n");

    //new socket address
    struct sockaddr_in client_addr;
    int client_addr_len = sizeof(client_addr);
    //returns a new socketfd 
    SOCKET client_socket = accept(server_fd, (struct sockaddr*)&client_addr, &client_addr_len);

    if(client_socket == INVALID_SOCKET) {
        printf("Accept failed with error code: %d\n", WSAGetLastError());
        closesocket(server_fd);
        WSACleanup();
        return 1;
    }

    printf("Client connected!\n");

    char buffer[512];
    while (1) {
        int bytes_received = recv(client_socket, buffer, sizeof(buffer) - 1, 0);
        if (bytes_received == SOCKET_ERROR) {
            printf("recv failed with error code: %d\n", WSAGetLastError());
            break;
        }
        if (bytes_received == 0) {
            printf("Client disconnected.\n");
            break;
        }
        buffer[bytes_received] = '\0';
        printf("Received from client: %s\n", buffer);

        if (strcmp(buffer, "quit\n") == 0) {
            printf("Client requested to quit.\n");
            break;
        }

        const char *reply = "MESSAGE RECEIVED!!!!!!\n";
        int bytes_sent = send(client_socket, reply, (int)strlen(reply), 0);
        if (bytes_sent == SOCKET_ERROR) {
            printf("send failed with error code: %d\n", WSAGetLastError());
            break;
        }
    }


    closesocket(client_socket);
    closesocket(server_fd);
    WSACleanup();


    return 0;
}

