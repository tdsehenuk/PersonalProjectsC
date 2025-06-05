#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <winsock2.h>


int main(int argc, char *argv[]) {

    WSADATA wsa;
    // initialize Winsock version 2.2, if it fails just die
    if (WSAStartup(MAKEWORD(2,2), &wsa) != 0) {
        printf("Failed to initialize Winsock. Error code: %d\n", WSAGetLastError());
        return 1;
    }

    // create a socket: IPv4, TCP, 0 means system picks correct protocol
    SOCKET server_fd = socket(AF_INET, SOCK_STREAM, 0);
    // socket returns an unsigned int so if it's -1, something definitely exploded
    if (server_fd == INVALID_SOCKET) {
        perror("socket failed");
        exit(EXIT_FAILURE);
    }

    struct sockaddr_in server_addr; 
    // fill out the struct for the server details (family, IP, port)
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY; // listen on any network interface
    server_addr.sin_port = htons(8080);       // port 8080, converted to network byte order

    // bind the socket to the IP and port above, or error out hard
    if(bind(server_fd, (struct sockaddr*)&server_addr, sizeof(server_addr)) == SOCKET_ERROR) {
        printf("Bind failed with error code: %d\n", WSAGetLastError());
        closesocket(server_fd);
        WSACleanup();
        return 1;
    }

    printf("Bind successful. Listening on port 8080...\n");

    // start listening for incoming connections, backlog set to SOMAXCONN (max size allowed)
    if(listen(server_fd, SOMAXCONN) == SOCKET_ERROR) {
        printf("Listen failed with error code: %d\n", WSAGetLastError());
        closesocket(server_fd);
        WSACleanup();
        return 1;
    }

    printf("Listening for incoming connections...\n");

    struct sockaddr_in client_addr;
    int client_addr_len = sizeof(client_addr);
    //accept incoming connection, returns a new socket for communication
    SOCKET client_socket = accept(server_fd, (struct sockaddr*)&client_addr, &client_addr_len);
    if(client_socket == INVALID_SOCKET) {
        printf("Accept failed with error code: %d\n", WSAGetLastError());
        closesocket(server_fd);
        WSACleanup();
        return 1;
    }

    printf("Client connected!\n");

    //infinite loop to handle messages
    while (1) {
        char buffer[512];
        //receive data from the client
        int bytes_received = recv(client_socket, buffer, sizeof(buffer) - 1, 0);
        if (bytes_received == SOCKET_ERROR) {
            printf("recv failed with error code: %d\n", WSAGetLastError());
            break;
        }
        if (bytes_received == 0) {
            printf("Client disconnected.\n");
            break;
        }
        //null-terminate the received string
        buffer[bytes_received] = '\0';
        printf("Received from client: %s\n", buffer);

        //if client sent 'exit', we're done
        if (strcmp(buffer, "exit") == 0) {
            printf("Client requested to quit.\n");
            break;
        }

        //prompt server user to type a reply message
        char send_msg[512];
        printf("\nYour message to the client: ");
        fgets(send_msg, sizeof(send_msg), stdin);
        send_msg[strcspn(send_msg, "\n")] = 0;  // get rid of newline at the end

        //send the reply to the client
        int bytes_sent = send(client_socket, send_msg, (int)strlen(send_msg), 0);
        if (bytes_sent == SOCKET_ERROR) {
            printf("send failed with error code: %d\n", WSAGetLastError());
            break;
        }
    }

    //cleanup close sockets and deinit Winsock
    closesocket(client_socket);
    closesocket(server_fd);
    WSACleanup();

    return 0;
}
