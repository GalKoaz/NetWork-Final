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

void printsin(struct sockaddr_in *sin, char *pname, char *msg) {
    printf("%s\n", pname);
    printf("%s: ", msg);
    printf("ip= %s, port= %d", inet_ntoa(sin->sin_addr), sin->sin_port);

    // -- port: sin->sin_port (host integer type)
    // -- IP: sin->sin_addr (IP in dotted-decimal notation)
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

    socket_fd = socket(AF_INET, SOCK_DGRAM, 0);

    bzero((char *) &s_in, sizeof(s_in));

    s_in.sin_family = (short) AF_INET;

    s_in.sin_addr.s_addr = htonl(INADDR_ANY);

    s_in.sin_port = htons(PORT);

    printsin(&s_in, "RECV_UDP", "Local socket is:");
    fflush(stdout);

    bind(socket_fd, (struct sockaddr *) &s_in, sizeof(s_in));

    for (;;) { // while TRUE
        fsize = sizeof(from); // file size

        recvfrom(socket_fd, &msg, sizeof(msg), 0, (struct sockaddr *) &from, &fsize);

        printsin(&from, "recv_udp: ", "Packet from:");
        printf("Got data ::%c%ld%c\n", msg.head, (long) ntohl(msg.body), msg.tail);

        fflush(stdout);
    }

    return 0;
}
