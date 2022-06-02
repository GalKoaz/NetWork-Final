//
// Created by gal on 5/17/22.
//

#include <sys/types.h>
#include <netinet/in.h>
#include <inttypes.h>
#include <netdb.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <strings.h>
#include <unistd.h>
#include <stdio.h>

#define PORT 3333

int main(int argc, char *argv[]) {
    int socket_fd;
    struct sockaddr_in dest;
    struct hostent *hostptr;
    struct {
        char head;
        u_long body;
        char tail;
    } msgbuf;

    /* Socket Creation with ( Internet domain, UDP, Protocol 0) */
    socket_fd = socket(AF_INET, SOCK_DGRAM, 0);

    /* Erases the data in the n bytes of the memory */
    bzero((char *) &dest, sizeof(dest)); /* They say you must do this */

    /* The gethostbyname() function returns a structure of type hostent for the given host name. */
    hostptr = gethostbyname(argv[1]);

    /********************* Destination *********************/
    dest.sin_family = (short) AF_INET;

    /*copies n bytes from src to dest. */
    bcopy(hostptr->h_addr, (char *) &dest.sin_addr, hostptr->h_length);

    dest.sin_port = htons(PORT);

    /* body integer increase every iteration we started from zero*/
    int body_increase = 0;

    for (;;) {
        msgbuf.head = '<'; // head
        msgbuf.body = htonl(body_increase); // body fill with the int /* IMPORTANT! */
        msgbuf.tail = '>'; // tail
        sendto(socket_fd, &msgbuf, sizeof(msgbuf), 0, (struct sockaddr *) &dest,
               sizeof(dest));
        body_increase++; // increment
        sleep(1); // one-second sleep
    }
    return 0;
}
