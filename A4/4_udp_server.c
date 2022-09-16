#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <dirent.h>
#include <time.h>
#include <sys/stat.h>
#include <sys/types.h>

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
    while(1){
        bzero(buff, 1024);
        recvfrom(server_socket, buff, 1024, 0, (struct sockaddr*)&client_address, &address_size);

        printf("Client: %s\n", buff);
        bzero(ch, 1024);
        strcpy(ch, "");
        
        if(strcmp(buff, "CDIR") == 0) {
            // Create Directory
            strcpy(ch, "Send PID");
            sendto(server_socket, ch, 1024, 0, (struct sockaddr*)&client_address, sizeof(client_address));
            bzero(buff, 1024);
            recvfrom(server_socket, buff, 1024, 0, (struct sockaddr*)&client_address, &address_size);
            int check = mkdir(buff, 0777);
            if(!check) strcpy(ch, "Directory Created.");
            else strcpy(ch, "Counldn't create directory.");
        } else if(strcmp(buff, "DELFILE") == 0) {
            // Delete File
            strcpy(ch, "Enter file name to delete.");
            sendto(server_socket, ch, 1024, 0, (struct sockaddr*)&client_address, sizeof(client_address));
            bzero(buff, 1024);
            recvfrom(server_socket, buff, 1024, 0, (struct sockaddr*)&client_address, &address_size);
            if(remove(buff) == 0) strcpy(ch, "Deleted successfully");
            else strcpy(ch, "Couldn't delete.");
        } else if(strcmp(buff, "DELDIR") == 0) {
            // Delete Directory
            strcpy(ch, "Send PID");
            sendto(server_socket, ch, 1024, 0, (struct sockaddr*)&client_address, sizeof(client_address));
            bzero(buff, 1024);
            recvfrom(server_socket, buff, 1024, 0, (struct sockaddr*)&client_address, &address_size);
            int check = rmdir(buff);
            if(check == 0) strcpy(ch, "Directory removed.");
            else strcpy(ch, "Directory not removed.");
        } else if(strcmp(buff, "DATE") == 0) {
            // Get Date
            time_t T = time(NULL);
            struct tm tm = *localtime(&T);
            char tmp[1024];
            sprintf(tmp, "%d-%02d-%02d %02d:%02d:%02d", tm.tm_year + 1900, tm.tm_mon+1, tm.tm_mday, tm.tm_hour, tm.tm_min, tm.tm_sec);
            strcpy(ch, tmp);
        } else if(strcmp(buff, "LIST") == 0) {
            // List Directory
            DIR *d;
            struct dirent *dir;
            d = opendir(".");
            if (d) {
                while ((dir = readdir(d)) != NULL) {
                    strcat(ch, dir->d_name);
                    strcat(ch, "\n");
                }
                closedir(d);
            }
        } else if(strcmp(buff, "QUIT") == 0) {
            // Exit
            strcpy(ch, "Thanks. You can exit.");
        } else {
            // Unknown
            strcpy(ch, "Unknown command.");
        }

        sendto(server_socket, ch, 1024, 0, (struct sockaddr*)&client_address, sizeof(client_address));
    }
    // printf("Data send: %s\n", buff);
    
    close(client_socket);
    printf("Client Disconnected!!\n");
    return 0;
}