#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

int main(){
    char *ip="127.0.0.1";
    int port = 8080;

    int server_socket, client_socket;
    struct sockaddr_in server_address, client_address;
    socklen_t address_size;
    char buff[1024],ch[1024];
    int n;

    server_socket = socket(AF_INET, SOCK_DGRAM, 0);
    if(server_socket<0){
        perror("Socket Error\n");
        exit(1);
    }
    printf("UDP server socket created\n");

    memset(&server_address, '\0', sizeof(server_address));
    server_address.sin_family = AF_INET;
    server_address.sin_port = port;
    server_address.sin_addr.s_addr = inet_addr(ip);

    n=bind(server_socket, (struct sockaddr*)& server_address, sizeof(server_address));
    if(n < 0) {
        perror("!Bind Error");
        exit(1);
    }
    printf("Bind to the port no. : %d\n", port);

    address_size = sizeof(client_address);
    int cnt=0;
    while(1){
        bzero(buff, 1024);
        recvfrom(server_socket, buff, 1024, 0, (struct sockaddr*)&client_address, &address_size);
        printf("Data recv from client-%d: %s\n", ++cnt,buff);

        bzero(ch, 1024);
        strcpy(ch, "Welcome to the UDP Server.");
        sendto(server_socket, ch, 1024, 0, (struct sockaddr*)&client_address, sizeof(client_address));
    }
    // printf("Data send: %s\n", buff);
    
    close(client_socket);
    printf("Client Disconnected!!\n");
    return 0;
}