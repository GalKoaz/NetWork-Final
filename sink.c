//
// Created by gal on 5/18/22.
//

#include <sys/types.h>
#include <netinet/in.h>
#include <inttypes.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <strings.h>
#include <unistd.h>
#include <stdio.h>

#define PORT 3333

void printsin(struct sockaddr_in *sin) {
    printf("From ip= %s, port= %d", inet_ntoa(sin->sin_addr), sin->sin_port);
//  -- port: sin->sin_port (host integer type)
//  -- IP: sin->sin_addr (IP in dotted-decimal notation)
    printf("\n");
}


int main(int argc, char *argv[]) {
    int socket_fd, cc, fsize;
    struct sockaddr_in s_in, from;
    struct {
        char head;
        u_long body;
        char tail;
    } msg;

    socket_fd = socket(AF_INET, SOCK_DGRAM, 0);

    bzero((char *) &s_in, sizeof(s_in));  /* They say you must do this    */

    s_in.sin_family = (short) AF_INET;
    s_in.sin_addr.s_addr = htonl(INADDR_ANY);    /* WILDCARD */
    s_in.sin_port = htons((u_short) 0x3333);

    fflush(stdout);

    bind(socket_fd, (struct sockaddr *) &s_in, sizeof(s_in));

    for (;;) {
        fsize = sizeof(from);
        printsin( &from);
        cc = recvfrom(socket_fd, &msg, sizeof(msg), 0, (struct sockaddr *) &from, &fsize);
        printf("The Message contains: %lu, the size of bytes is: %d\n",msg.body, cc);
        printf("Got data ::%c%ld%c\n", msg.head, (long) ntohl(msg.body), msg.tail);
        fflush(stdout);
    }
    return 0;
}
