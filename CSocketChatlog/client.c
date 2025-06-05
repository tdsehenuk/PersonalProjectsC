#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <winsock2.h>

int main(int argc, char *argv[]) {

    WSADATA wsa;
    if(WSAStartup(MAKEWORD(2,2), &wsa) != 0) {
         printf("\nWSAStartup failed. Error code: %d\n", WSAGetLastError());
        return 1;
    }

    SOCKET client_socket = socket(AF_INET, SOCK_STREAM, 0);
    if(client_socket == INVALID_SOCKET) {
        printf("\nSocket Creation failed. Error code %d", WSAGetLastError());
        WSACleanup();
        return 1;
    }

    struct sockaddr_in server_addr;
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(8080);
    server_addr.sin_addr.s_addr = inet_addr("127.0.0.1");


    if(connect(client_socket, (struct sockaddr*)&server_addr, sizeof(server_addr)) == SOCKET_ERROR) {
        printf("\nThere was an error connecting to the server. Error code %d", WSAGetLastError());
        closesocket(client_socket);
        return 1;
    } 

    printf("\nConnected to server!\n");

    //const char *msg = "\nHello from client";
  while (1) {
        char msg[512];
        printf("Enter message (type 'exit' to quit): ");
        fgets(msg, sizeof(msg), stdin);
        msg[strcspn(msg, "\n")] = 0;  // remove newline

        if (strcmp(msg, "exit") == 0)
            break;

        int bytes_sent = send(client_socket, msg, (int)strlen(msg), 0);
        if (bytes_sent == SOCKET_ERROR) {
            printf("Send failed. Error code %d\n", WSAGetLastError());
            break;
        }

        char buffer[512];
        int bytes_received = recv(client_socket, buffer, sizeof(buffer) - 1, 0);
        if (bytes_received == SOCKET_ERROR) {
            printf("Receive failed. Error code %d\n", WSAGetLastError());
            break;
        } else if (bytes_received == 0) {
            printf("Server closed the connection.\n");
            break;
        } else {
            buffer[bytes_received] = '\0';
            printf("Server replied: %s\n", buffer);
        }
}


    closesocket(client_socket);
    WSACleanup();

    return 0;
}