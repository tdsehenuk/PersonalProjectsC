#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <winsock2.h>

int main(int argc, char *argv[]) {

    WSADATA wsa;
    if(WSAStartup(MAKEWORD(2,2), &wsa != 0)) {
         printf("WSAStartup failed. Error code: %d\n", WSAGetLastError());
        return 1;
    }

    SOCKET client_socket = socket(AF_INET, SOCK_STREAM, 0);
    if(client_socket == INVALID_SOCKET) {
        printf("Socket Creation failed. Error code %s", WSAGetLastError());
        WSACleanup();
        return 1;
    }

    struct sockaddr_in server_addr;
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(8080);
    server_addr.sin_addr.s_addr = inet_addr("127.0.0.1");


    if(connect(client_socket, (struct sockaddr*)&server_addr, sizeof(server_addr)) == SOCKET_ERROR) {
        printf("There was an error connecting to the server. Error code %s", WSAGetLastError());
        closesocket(client_socket);
        return 1;
    } 

    printf("Connected to server!\n");

    const char *msg = "Hello from client";
    int bytes_sent = send(client_socket, msg, (int)(strlen(msg)), 0 );
    
    if(bytes_sent == SOCKET_ERROR) {
        printf("Send failed. Error code %s" , WSAGetLastError());
        closesocket(client_socket);
        WSACleanup();
        return 1;
    }

    printf("Message was sent to the server");

    char buffer[512];
    int bytes_recieved = recv(client_socket, buffer, sizeof(buffer) -1 , 0);
    if(bytes_recieved == SOCKET_ERROR) {
        printf("Recieved failed. Error code %d", WSAGetLastError);
    } else if(bytes_recieved == 0) {
        printf("Connection was closed by the server. Error code %s", WSAGetLastError());
    } else {
        buffer[bytes_recieved] = '\0';
        printf("server replied: %s", buffer);
    }

    closesocket(client_socket);
    WSACleanup();

    return 0;
}