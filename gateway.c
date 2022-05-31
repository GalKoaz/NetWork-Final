#include <sys/types.h>
#include <netinet/in.h>
#include <inttypes.h>
#include <netdb.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <strings.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>

#define PORT 3333
#define PORT2 3334

int main(int argc, char *argv[]) {
    int socket_sc, socket_ds, fsize; // unique identifier (handle) for a file or other input/output resource
    struct sockaddr_in s_in, from, dest; // Socket address in address, Socket destination address
    struct hostent *hostptr; // Host pointer
    struct {
        char head;
        u_long body;
        char tail;
    } msgbuf; // Struct : char of head unsigned number of body (The process number) and char tail.


    /* Socket Creation with ( Internet domain, UDP, Protocol 0) */
    socket_sc = socket(AF_INET, SOCK_DGRAM, 0);
    socket_ds = socket(AF_INET, SOCK_DGRAM, 0);

    /* Erases the data in the n bytes of the memory */
    bzero((char *) &dest, sizeof(dest));
    bzero((char *) &s_in, sizeof(s_in));

    /* The gethostbyname() function returns a structure of type hostent for the given host name. */
    hostptr = gethostbyname(argv[1]);


    dest.sin_family = (short) AF_INET;
    bcopy(hostptr->h_addr, (char *) &dest.sin_addr, hostptr->h_length);
    dest.sin_port = htons(PORT2);

    s_in.sin_family = (short) AF_INET;
    s_in.sin_addr.s_addr = htonl(INADDR_ANY);
    s_in.sin_port = htons(PORT);


    bind(socket_sc, (struct sockaddr *) &s_in, sizeof(s_in));

    srandom(333);
    for (;;) {
        fsize = sizeof(from);
        recvfrom(socket_sc, &msgbuf, sizeof(msgbuf), 0, (struct sockaddr *) &from, &fsize);
        //printf("%f\n",((float) random()) / ((float) RAND_MAX));
        float rand = (((float) random()) / ((float) RAND_MAX));
        if (rand > 0.5) {
            printf("Value Random: %f > 0.5\n",rand);
            printf("Got data ::%c%ld%c\n", msgbuf.head, (long) htonl(msgbuf.body), msgbuf.tail);
            printf("ID Process ::%c%ld%c\n", msgbuf.head, (long) getpid(), msgbuf.tail);
            sendto(socket_ds, &msgbuf, sizeof(msgbuf), 0, (struct sockaddr *) &dest, sizeof(dest));
        }
    }
    return 0;
}