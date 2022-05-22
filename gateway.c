#include <sys/types.h>
#include <netinet/in.h>
#include <inttypes.h>
#include <netdb.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <strings.h>
#include <unistd.h>
#include <stdlib.h>

#define PORT 3333

int main(int argc, char *argv[]) {
    int socket_sc, socket_ds, fsize; // unique identifier (handle) for a file or other input/output resource
    struct sockaddr_in dest; // Socket destination address
    struct sockaddr_in s_in, from; // Socket address in address
    struct hostent *hostptr; // Host pointer
    struct {
        char head;
        u_long body;
        char tail;
    } msgbuf; // Struct : char of head unsigned number of body (The process number) and char tail.

    /* Socket Creation with ( Internet domain , UDP , Protocol 0) */
    socket_sc = socket(AF_INET, SOCK_DGRAM, 0);
    socket_ds = socket(AF_INET, SOCK_DGRAM, 0);

    /* Erases the data in the n bytes of the memory */
    bzero((char *) &dest, sizeof(dest));
    bzero((char *) &s_in, sizeof(s_in));

    /* The gethostbyname() function returns a structure of type hostent for the given host name. */
    hostptr = gethostbyname(argv[1]);


    dest.sin_family = (short) AF_INET;
    bcopy(hostptr->h_addr, (char *) &dest.sin_addr, hostptr->h_length);
    dest.sin_port = htons(PORT + 1);

    s_in.sin_family = (short) AF_INET;
    s_in.sin_addr.s_addr = htonl(INADDR_ANY);
    s_in.sin_port = htons(PORT);

    bind(socket_sc, (struct sockaddr *) &s_in, sizeof(s_in));


    for (;;) {
        fsize = sizeof(from);
        recvfrom(socket_sc, &msgbuf, sizeof(msgbuf), 0, (struct sockaddr *) &from, &fsize);
        float rand = ((float) random()) / ((float) RAND_MAX);
        srandom(333);
        if (rand > 0.5) {
            //the datagram recived is fordwarded onto the port P+1
            msgbuf.head = '<'; // define the head
            msgbuf.body = htonl(getpid());
            msgbuf.tail = '>'; // define the tail
            sendto(socket_ds, &msgbuf, sizeof(msgbuf), 0, (struct sockaddr *) &dest, sizeof(dest));
        }
    }
    return 0;
}