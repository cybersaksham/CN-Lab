#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#define MAX_SIZE 5

int main(){
    char *ip="127.0.0.1";
    int port = 5566;
    int a;

    int server_socket, client_socket;
    struct sockaddr_in server_address, client_address;
    socklen_t address_size;
    char buff[1024], ch[1024];
    int n;

    server_socket = socket(AF_INET, SOCK_STREAM, 0);
    if(server_socket<0){
        perror("Socket Error\n");
        exit(1);
    }
    printf("TCP server socket created\n");

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

    listen(server_socket, 5);
    printf("Listening.....\n");

    int cnt=0;
    while(1) {
        address_size = sizeof(client_address);
        client_socket = accept(server_socket, (struct sockaddr*)&client_address, &address_size);
        bzero(buff, 1024);
        a = recv(client_socket, buff, sizeof(buff), 0);
        if(a == 0){
            break;
        }
        printf("Client-%d: %s\n", ++cnt, buff);
        bzero(ch,1024);
        strcpy(ch,"Hey!, This is server");
        send(client_socket,ch,strlen(ch),0);
    }

    close(client_socket);
    printf("Client Disconnected!!\n");

    return 0;
}