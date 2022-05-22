#include <sys/types.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <signal.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <netinet/in.h>

#define SIM_LENGTH 10
#define PORT 3333

int main(void) {
    int sock;  // the socket
    int connect_sock; // connected number socket
    struct sockaddr_in serv_name;  // socket address server name
    size_t len; // length
    int count; // counter

    /* Create the socket */
    sock = socket(AF_INET, SOCK_STREAM, 0);

    /* fill in all the info for the server */
    bzero(&serv_name, sizeof(serv_name));
    serv_name.sin_family = AF_INET;
    serv_name.sin_port = htons(PORT);

    /* The process of allocating a port number to a socket is called 'binding'.*/
    bind(sock, (struct sockaddr *) &serv_name, sizeof(serv_name));

    /* The listen() function accepts a queue size through the parameter backlog.*/
    listen(sock, 1);

    /* update the length size of the server name */
    len = sizeof(serv_name);

    /* Connect the socket */
    connect_sock = accept(sock, (struct sockaddr *) &serv_name, &len);

    /* Write SIM Length to socket */
    for (count = 1; count <= SIM_LENGTH; count++) {
        write(connect_sock, &count, 4);
        printf("Server has written %d to socket.\n", count);
    }

    /* Close the socket */
    close(connect_sock);

    /* Close the socket */
    close(sock);

}
