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

#define PORT 3333

int main(int argc, char *argv[])
{
    int socket_fd;
    struct sockaddr_in  dest;
    struct hostent *hostptr;
    struct { char head; u_long body; char tail; } msgbuf;

    socket_fd = socket (AF_INET, SOCK_DGRAM, 0);
    bzero((char *) &dest, sizeof(dest)); /* They say you must do this */
    hostptr = gethostbyname(argv[1]);
    dest.sin_family = (short) AF_INET;
    bcopy(hostptr->h_addr, (char *)&dest.sin_addr,hostptr->h_length);
    dest.sin_port = htons(PORT);

    for(;;){
        int body_increase = 0;
        msgbuf.head = '<';
        msgbuf.body =  htonl(body_increase);/* IMPORTANT! */
        msgbuf.tail = '>';
        sendto(socket_fd,&msgbuf,sizeof(msgbuf),0,(struct sockaddr *)&dest,
               sizeof(dest));
        body_increase++;
        sleep(1);
    }

    return 0;
}