
#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <netinet/in.h>
#include <arpa/inet.h>

int main(int argc, char* argv[]) {
  struct addrinfo* res; // address info pointer
  char* hostname; // pointer to hostname
  char* hostaddr; // pointer to hostname
  struct sockaddr_in* saddr; // socket address pointer

  /* Check if the User enter hostname lookup in the commend-line */
  if (argc != 2) {
    perror("Usage: hostnamelookup <hostname>\n");
    exit(1);
  }

  /* take the full hostname from the commend-line*/
  hostname = argv[1];

  /* Check if we have error in resolving hostname*/
  if (0 != getaddrinfo(hostname, NULL, NULL, &res)) {
    fprintf(stderr, "Error in resolving hostname %s\n", hostname);
    exit(1);
  }

  /* fill in all the info for the ns lookup */
  saddr = (struct sockaddr_in*)res->ai_addr;
  hostaddr = inet_ntoa(saddr->sin_addr);

  /* Print the address hostname and host address*/
  printf("Address for %s is %s\n", hostname, hostaddr);

  /* Exit the program*/
  exit(0);
}
