#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <winsock2.h>


int main(int argc, char *argv[]) {
    
    
    //IPv4, TCP, system chooses protocol
    int server_fd = socket(AF_INET, SOCK_STREAM, 0);

    //socket() returns a unsigned int so if its value is -1 it failed
    if (server_fd == "INVALID_SOCKET") {
        perror("socket failed");
        exit(EXIT_FAILURE);
    }


    return 0;
}

