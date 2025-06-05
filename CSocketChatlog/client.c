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

    return 0;
}