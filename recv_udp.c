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
    /* Socket Creation with ( Internet domain , UDP , Protocol 0) */

    bzero((char *) &s_in, sizeof(s_in));
    /* They say you must do this */
    /*The bzero() function erases the data in the n bytes of the memory
     starting at the location pointed to by s, by writing zeros (bytes
    containing '\0') to that area.*/

    s_in.sin_family = (short) AF_INET;
    /* The address family for the transport address. This member should always be set to AF_INET. */

    s_in.sin_addr.s_addr = htonl(INADDR_ANY);
    /* WILDCARD */
    /* sin_addr is the IP address in the socket (the socket structure also contains other data, such as a port).
    * The type of sin_addr is a union, so it can be accessed in three different ways : as s_un_b (four 1-byte integers),
    * s_un_w (two 2-bytes integers) or as s_addr (one 4-bytes integer).
    * The htonl() function converts the unsigned integer hostlong from host byte order to network byte order.
    * getpid() returns the process ID (PID) of the calling process.
    * (This is often used by routines that generate unique temporary filenames.)
    *  ---------------> We accept connections from ANY internet address <---------------
    */

    s_in.sin_port = htons((u_short) 0x3333);
    /* The sin_port is a transport protocol port number.
    * The htons() function translates a short integer from host byte order to network byte order.
     * Parameter Description a. The unsigned short integer to be put into network byte order. */

    printsin( &s_in, "RECV_UDP", "Local socket is:");
    fflush(stdout);
    /* fflush(stdout) is typically used for output stream only. Its purpose is to clear (or flush)
     * the output buffer and move the buffered data to console (in case of stdout)
     * or disk (in case of file output stream). Below is its syntax.*/

    bind(socket_fd, (struct sockaddr *) &s_in, sizeof(s_in));
    /* The bind() function binds a unique local name to the socket with descriptor socket.
     * the bind use socket_fd and the address socket in and the size*/

    for (;;) { // while TRUE
        fsize = sizeof(from); // file size

        cc = recvfrom(socket_fd, &msg, sizeof(msg), 0, (struct sockaddr *) &from, &fsize);
        /*
         *The recvfrom() calls are used to receive messages from a socket,
         * and may be used to receive data on a socket whether or not it is connection-oriented.
         */

        printsin( &from, "recv_udp: ", "Packet from:");
        printf("Got data ::%c%ld%c\n", msg.head, (long) ntohl(msg.body), msg.tail);
        // print "Got data::< id process >"

        fflush(stdout);
        /* fflush(stdout) is typically used for output stream only. Its purpose is to clear (or flush)
        * the output buffer and move the buffered data to console (in case of stdout)
        * or disk (in case of file output stream). Below is its syntax.*/
    }

    return 0;
}

