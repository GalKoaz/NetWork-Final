//
// Created by gal on 5/18/22.
//


#include <sys/types.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <signal.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>

#define SIM_LENGTH 10
#define IP_ADDRESS "127.0.0.1"
#define PORT 3333


int main(int argc, char *argv[]) {
    int sock;
    struct sockaddr_in cli_name;
    int count;
    int value;

    struct addrinfo *res;
    char *hostname;
    char *hostaddr;
    struct sockaddr_in *saddr;

    char ip[100];
    int port = -1;
    char path[100];
    char *url;


    if (argc != 2) {
        perror("Usage: client <hostname>\n");
        exit(1);
    }


    //hostname = argv[1];
    url = argv[1];
    int flag = 0;
    int scan = -1;
    int counter = 0;
    for (int i = 0; i < sizeof(url) / sizeof(int); ++i) {
        if (url[i] == ':') {
            counter++;
            if(counter == 2){
                flag = 1;
                scan = sscanf(url, "%*[^:]%*[:/]%[^:]:%d%s", ip, &port, path);
                break;
            }
        }
    }
    if(counter != 2){

    }

    printf("Client is alive and establishing socket connection.\n");


    sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock < 0) {
        perror("Error opening channel");
        close(sock);
        exit(1);
    }

    if (0 != getaddrinfo(hostname, NULL, NULL, &res)) {
        fprintf(stderr, "Error in resolving hostname %s\n", hostname);
        exit(1);
    }


    saddr = (struct sockaddr_in *) res->ai_addr;
    hostaddr = inet_ntoa(saddr->sin_addr);


    bzero(&cli_name, sizeof(cli_name));
    cli_name.sin_family = AF_INET;
    cli_name.sin_addr.s_addr = inet_addr(ip);
    cli_name.sin_port = htons(port);


    if (connect(sock, (struct sockaddr *) &cli_name, sizeof(cli_name)) < 0) {
        perror("Error establishing communications");
        close(sock);
        exit(1);
    }

    printf("GET %s HTTP/1.0\n",url);
    printf("HOST: %s\n\n",ip);

    for (count = 1; count <= SIM_LENGTH; count++) {
        read(sock, &value, 4);
        printf("Client has received %d from socket.\n", value);
    }
    printf("The host address is %s\n", hostaddr);
    printf("Exiting now.\n");

    close(sock);
    exit(0);
}
