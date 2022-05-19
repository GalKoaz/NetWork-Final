
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

int main(int argc, char* argv[]) {
    int sock;
    struct sockaddr_in cli_name;
    int count;
    int value;

    struct addrinfo* res;
    char* hostname;
    char* hostaddr;
    struct sockaddr_in* saddr;

    if (argc != 2) {
        perror("Usage: client <hostname>\n");
        exit(1);
    }

    hostname = argv[1];


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


    saddr = (struct sockaddr_in*)res->ai_addr;
    hostaddr = inet_ntoa(saddr->sin_addr);


    bzero(&cli_name, sizeof(cli_name));
    cli_name.sin_family = AF_INET;
    cli_name.sin_addr.s_addr = inet_addr(hostaddr);
    cli_name.sin_port = htons(PORT);


    if (connect(sock, (struct sockaddr *) &cli_name, sizeof(cli_name)) < 0) {
        perror("Error establishing communications");
        close(sock);
        exit(1);
    }


    for (count = 1; count <= SIM_LENGTH; count++) {
        read(sock, &value, 4);
        printf("Client has received %d from socket.\n", value);
    }
    printf("The host address is %s\n", hostaddr);
    printf("Exiting now.\n");

    close(sock);
    exit(0);

} 
