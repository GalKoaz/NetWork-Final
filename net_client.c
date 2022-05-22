
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
// #define IP_ADDRESS "127.0.0.1" // not use any more
#define PORT 3333

int main(int argc, char *argv[]) {
    int sock; // socket number
    struct sockaddr_in cli_name; // socket address, client name
    int count;  // counter
    int value;  // value of the buffer

    struct addrinfo *res;  // address info
    char *hostname;  // hostname
    char *hostaddr;  // host address
    struct sockaddr_in *saddr;  // socket address


    /* Check if the User enter hostname in the commend-line */
    if (argc != 2) {
        perror("Usage: client <hostname>\n");
        exit(1);
    }

    /* take the hostname from the commend-line */
    hostname = argv[1];


    printf("Client is alive and establishing socket connection.\n");

    /* Create the socket */
    sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock < 0) {
        perror("Error opening channel");
        close(sock);
        exit(1);
    }

    /* Check if we have error in resolving hostname*/
    if (0 != getaddrinfo(hostname, NULL, NULL, &res)) {
        fprintf(stderr, "Error in resolving hostname %s\n", hostname);
        exit(1);
    }

    /* Receive the inet from the socket address */
    saddr = (struct sockaddr_in *) res->ai_addr;
    hostaddr = inet_ntoa(saddr->sin_addr);

    /* fill in all the info for the client */
    bzero(&cli_name, sizeof(cli_name));
    cli_name.sin_family = AF_INET;
    cli_name.sin_addr.s_addr = inet_addr(hostaddr);
    cli_name.sin_port = htons(PORT);

    /* Connect the Socket */
    if (connect(sock, (struct sockaddr *) &cli_name, sizeof(cli_name)) < 0) {
        perror("Error establishing communications");
        close(sock);
        exit(1);
    }

    /* Receive SIM Length from socket */
    for (count = 1; count <= SIM_LENGTH; count++) {
        read(sock, &value, 4);
        printf("Client has received %d from socket.\n", value);
    }
    printf("The host address is %s\n", hostaddr);
    printf("Exiting now.\n");

    /* Close the socket */
    close(sock);

    /* Exit the program */
    exit(0);

} 
