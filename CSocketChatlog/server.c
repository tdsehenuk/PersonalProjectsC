#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <winsock2.h>


int main(int argc, char *argv[]) {

    //IPv4, TCP, system chooses protocol
    int server_fd = socket(AF_INET, SOCK_STREAM, 0);
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









    return 0;
}

