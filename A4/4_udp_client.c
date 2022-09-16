#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <time.h>

int main(){
    char *ip="127.0.0.1";
    int port = 8080;

    int sock;
    struct sockaddr_in address;
    socklen_t address_size;
    char buff[1024], ch[1024];
    int n;

    sock = socket(AF_INET, SOCK_DGRAM, 0);
    if(sock<0){
        perror("Socket Error\n");
        exit(1);
    }
    printf("UDP server socket created\n");

    memset(&address, '\0', sizeof(address));
    address.sin_family = AF_INET;
    address.sin_port = port;
    address.sin_addr.s_addr = inet_addr(ip);

    while(1){
        bzero(buff, 1024);
        scanf("%s", buff);
        sendto(sock, buff, 1024, 0, (struct sockaddr*)&address, sizeof(address));

        if(strcmp(buff, "CDIR") == 0 || strcmp(buff, "DELDIR") == 0) {
            recvfrom(sock, ch, 1024, 0, (struct sockaddr*)&address, &address_size);
            sprintf(buff, "%d", getpid());
            sendto(sock, buff, 1024, 0, (struct sockaddr*)&address, sizeof(address));
        }

        bzero(ch, 1024);
        address_size = sizeof(address);
        recvfrom(sock, ch, 1024, 0, (struct sockaddr*)&address, &address_size);
        printf("Server: %s\n", ch);

        if(strcmp(buff, "QUIT") == 0) break;
    }
    

    close(sock);
    printf("Disconnected from the server\n");
    return 0;
}