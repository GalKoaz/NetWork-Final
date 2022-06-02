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

#define PORT2 3334

void printsin(struct sockaddr_in *sin, char *pname, char *msg) {
    //printf("%s\n", pname);
    printf("%s: ", msg);
    printf("ip= %s, port= %d", inet_ntoa(sin->sin_addr), sin->sin_port);
    printf("\n");
}


int main(int argc, char *argv[]) {
    int socket_fd, cc, fsize;  // Socket number , cc , and file size
    struct sockaddr_in s_in, from; // Socket destination and sources address
    struct {
        char head;
        u_long body;
        char tail;
    } msg; // Struct : char of head unsigned number of body (The process number) and char tail.

    /* Socket Creation with ( Internet domain, UDP, Protocol 0) */
    socket_fd = socket(AF_INET, SOCK_DGRAM, 0);

    /* Erases the data in the n bytes of the memory */
    bzero((char *) &s_in, sizeof(s_in));

    /********************** Incoming **********************/
    s_in.sin_family = (short) AF_INET;

    s_in.sin_addr.s_addr = htonl(INADDR_ANY); // accept any address

    s_in.sin_port = htons(PORT2); // port 2 = port + 1

    /* binds a unique local name to the socket */
    bind(socket_fd, (struct sockaddr *) &s_in, sizeof(s_in));

    for (;;) { // while TRUE
        fsize = sizeof(from); // file size
        /* recvfrom from the Incoming to fill the message buffer */
        recvfrom(socket_fd, &msg, sizeof(msg), 0, (struct sockaddr *) &from, &fsize);

        /* Printed all the Message information */
        printsin(&from, "recv_udp: ", "Packet from:");
        printf("Got data ::%c%ld%c\n", msg.head, (long) htonl(msg.body), msg.tail);
        printf("ID process ::%c%ld%c\n", msg.head, (long) getpid(), msg.tail);
        fflush(stdout);
    }

    return 0;
}
