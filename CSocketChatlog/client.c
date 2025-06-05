#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <winsock2.h>

int main(int argc, char *argv[]) {

    WSADATA wsa;
    // initialize Winsock version 2.2, crash if it fails
    if(WSAStartup(MAKEWORD(2,2), &wsa) != 0) {
         printf("\nWSAStartup failed. Error code: %d\n", WSAGetLastError());
        return 1;
    }

    //create socket: IPv4, TCP, 0 means system picks protocol
    SOCKET client_socket = socket(AF_INET, SOCK_STREAM, 0);
    if(client_socket == INVALID_SOCKET) {
        printf("\nSocket Creation failed. Error code %d", WSAGetLastError());
        WSACleanup();
        return 1;
    }

    //fill in server address (localhost, port 8080)
    struct sockaddr_in server_addr;
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(8080); // port 8080 in network byte order
    server_addr.sin_addr.s_addr = inet_addr("127.0.0.1"); // loopback IP

    //try connecting to the server, error out if it fails
    if(connect(client_socket, (struct sockaddr*)&server_addr, sizeof(server_addr)) == SOCKET_ERROR) {
        printf("\nThere was an error connecting to the server. Error code %d", WSAGetLastError());
        closesocket(client_socket);
        return 1;
    } 

    printf("\nConnected to server!\n");

    //start chat loop
    while (1) {
        char msg[512];
        printf("Enter message (type 'exit' to quit): ");
        fgets(msg, sizeof(msg), stdin);
        msg[strcspn(msg, "\n")] = 0;  // remove newline if it's there

        //user wants to exit
        if (strcmp(msg, "exit") == 0)
            break;

        //send message to the server
        int bytes_sent = send(client_socket, msg, (int)strlen(msg), 0);
        if (bytes_sent == SOCKET_ERROR) {
            printf("Send failed. Error code %d\n", WSAGetLastError());
            break;
        }

        //wait for server reply
        char buffer[512];
        int bytes_received = recv(client_socket, buffer, sizeof(buffer) - 1, 0);
        if (bytes_received == SOCKET_ERROR) {
            printf("Receive failed. Error code %d\n", WSAGetLastError());
            break;
        } else if (bytes_received == 0) {
            printf("Server closed the connection.\n");
            break;
        } else {
            buffer[bytes_received] = '\0'; // null-terminate
            printf("Server replied: %s\n", buffer);
        }
    }

    //cleanup time
    closesocket(client_socket);
    WSACleanup();

    return 0;
}
