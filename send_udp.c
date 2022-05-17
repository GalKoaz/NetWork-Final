#include <sys/types.h>
#include <netinet/in.h>
#include <inttypes.h>
#include <netdb.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <strings.h>
#include <unistd.h>

int main(int argc, char *argv[]) {
    int socket_fd; // unique identifier (handle) for a file or other input/output resource
    struct sockaddr_in dest; // Socket destination address
    struct hostent *hostptr; // Host pointer
    struct {
        char head;
        u_long body;
        char tail;
    } msgbuf; // Struct : char of head unsigned number of body (The process number) and char tail.

    socket_fd = socket(AF_INET, SOCK_DGRAM, 0);
    /* Socket Creation with ( Internet domain , UDP , Protocol 0) */

    bzero((char *) &dest, sizeof(dest));
    /* They say you must do this */
    /*The bzero() function erases the data in the n bytes of the memory
     starting at the location pointed to by s, by writing zeros (bytes
    containing '\0') to that area.*/

    hostptr = gethostbyname(argv[1]);
    /* The gethostbyname() function returns a structure of type hostent for the given host name. */

    dest.sin_family = (short) AF_INET;
    /* The address family for the transport address. This member should always be set to AF_INET. */

    bcopy(hostptr->h_addr, (char *) &dest.sin_addr, hostptr->h_length);
    /* The bcopy() function copies n bytes from src to dest.  The result is correct, even when both areas overlap */

    dest.sin_port = htons((u_short) 0x3333);
    /* The sin_port is a transport protocol port number.
     * The htons() function translates a short integer from host byte order to network byte order.
     * Parameter Description a. The unsigned short integer to be put into network byte order. */

    msgbuf.head = '<'; // define the head

    msgbuf.body = htonl(getpid());
    /* IMPORTANT! */
    /* define the body:
     * The htonl() function converts the unsigned integer hostlong from host byte order to network byte order.
     * getpid() returns the process ID (PID) of the calling process.
     * (This is often used by routines that generate unique temporary filenames.)
     */

    msgbuf.tail = '>'; // define the tail

    sendto(socket_fd, &msgbuf, sizeof(msgbuf), 0, (struct sockaddr *) &dest, sizeof(dest));
    /* sendto() used to transmit a message to another socket.
     * sendto() is used on a connection-mode (SOCK_STREAM, SOCK_SEQPACKET) socket, the arguments
     * dest_addr and addrlen are ignored (and the error EISCONN may be returned when they are not NULL and 0),
     * and the error ENOTCONN is returned when the socket was not actually connected. Otherwise,
     * the address of the target is given by dest_addr with addrlen specifying its size. For sendmsg(),
     * the address of the target is given by msg.msg_name, with msg.msg_namelen specifying its size.
     */
    return 0;
}
