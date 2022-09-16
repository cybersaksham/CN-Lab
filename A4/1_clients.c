#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <time.h>

int main(){

        char *ip="127.0.0.1";
        int port = 5566;

        int sock;
        struct sockaddr_in address;
        socklen_t address_size;
        char buff[1024], ch[1024];
        int n;

        sock = socket(AF_INET, SOCK_STREAM, 0);
        if(sock<0){
            perror("Socket Error\n");
            exit(1);
        }
        printf("TCP server socket created\n");

        memset(&address, '\0', sizeof(address));
        address.sin_family = AF_INET;
        address.sin_port = port;
        address.sin_addr.s_addr = inet_addr(ip);

        connect(sock, (struct sockaddr*)&address, sizeof(address));
        printf("Connected to the server\n");
        clock_t start, end;
        float totalTime;
        int cnt=0;
        while(1){
            bzero(buff, 1024);
            scanf("%s", buff);
            printf("%d\n",++cnt);
            start=clock();
            send(sock, buff, strlen(buff), 0);

            bzero(ch, 1024);
            recv(sock, ch, sizeof(ch), 0);
            end = clock();
            printf("Server: %s\n", ch);
            totalTime = (float)(end - start) / (CLOCKS_PER_SEC);
            printf("round trip time is equal to: %f\n", totalTime);
        }


        close(sock);
        printf("Disconnected from the server\n");

    return 0;
}