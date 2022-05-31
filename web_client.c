//
// Created by gal on 5/18/22.
//

#include <sys/types.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>


int main(int argc, char *argv[]) {

    int sock;  // the socket
    struct sockaddr_in serv_addr; // socket address

    char hostname[255]; // hostname
    int port = 80; //default port 80 HTTP
    char path[1024]; // path in the url
    char *url;  // the url pointer
    char buffer[4600]; // the full buffer
    ssize_t bytes; // bytes read / writes
    int FLAG = 0; // FLAG checker 0/1

    struct hostent *server; // hostname to server
    char *http_query; // http query

    /* Check if the User enter Url in the commend-line */
    if (argc != 2) {
        perror("Usage: Web URL form ex. http://google.com\n");
        exit(1);
    }

    /* take the full url from the commend-line */
    url = argv[1];

    /* URL PARSER works on both methods:
     * 1.<protocol>://<hostname>/<path>
     * 2.<protocol>://<hostname>:port/<path>*/
    int counter = 0;
    for (int i = 0; i < sizeof(url) / sizeof(int); ++i) {
        if (url[i] == ':') {
            counter++;
            if (counter == 2) {
                sscanf(url, "%*[^:]%*[:/]%[^:]:%d%s", hostname, &port, path);
                break;
            }
        }
    }
    if (counter != 2) {
        sscanf(url, "%*[^:]%*[:/]%[^/]%s", hostname, path);
        printf("size of path %lu\n", strlen(path));
        if (strlen(path) == 0) {
            FLAG = 1;
        }
        //printf("%s\n%d\n%s\n",hostname, port, path);
    }

    printf("Web is alive and establishing socket connection.\n");

    /* Create the socket */
    sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock < 0) {
        perror("Error opening channel");
        close(sock);
        exit(1);
    }

    /* lookup for the ip address */
    server = gethostbyname(hostname);
    if (server == NULL) perror("ERROR, NO SUCH HOST!");
    printf("TEST %s TEST\n", hostname);


    /* fill in all the info for the server */
    memset(&serv_addr, 0, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(port);
    memcpy(&serv_addr.sin_addr.s_addr, server->h_addr, server->h_length);

    /* Connect the Socket */
    if (connect(sock, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) < 0) {
        perror("Error establishing communications");
        close(sock);
        exit(1);
    }

    /* HTTP Query utils (check for size and create the query) */
    int http_query_size = 0;
    if (FLAG) {
        http_query_size += strlen("GET / HTTP/1.0\r\n");
    } else {
        http_query_size += strlen("GET %s HTTP/1.0\r\n");
    }
    http_query_size += strlen("GET %s HTTP/1.0\r\n");
    http_query_size += strlen("HOST: %s\r\n");
    http_query_size += strlen("\r\n");
    printf("size of query: %d\n", http_query_size);
    http_query = (char *) malloc(http_query_size); // allocate memory for the query
    if (FLAG) {
        sprintf(http_query, "GET / HTTP/1.0\r\nHOST: %s\r\n", hostname);
    } else {
        sprintf(http_query, "GET %s HTTP/1.0\r\nHOST: %s\r\n", path, hostname);
    }
    //sprintf(http_query, "GET %s HTTP/1.0\r\nHOST: %s\r\n", path, hostname);
    strcat(http_query, "\r\n");

    /* ---------------------Send The Request---------------------- */
    unsigned long total = strlen(http_query);
    printf("%s\n", http_query);
    int sent = 0;
    do {
        printf("Write\n");
        bytes = write(sock, http_query + sent, total - sent);
        if (bytes < 0) {
            perror("Error writing message to socket");
        }
        if (bytes == 0) {
            printf("Writing all data is successful!\n");
            break;
        }
        sent += bytes;
    } while (sent < total);

    /* ---------------------Receive The Response---------------------- */
    memset(buffer, 0, sizeof(buffer));
    total = sizeof(buffer) - 1;
    int received = 0;
    do {
        bytes = read(sock, buffer + received, total - received);
        fprintf(stdout, "%s\n", buffer);
        fflush(stdout);
        if (bytes < 0) {
            perror("Error reading response from socket");
        }
        if (bytes == 0) {
            printf("All data is read successfully!\n");
            break;
        }
        received += bytes;
    } while (received < total);

    /* Checking if the storage is full from the socket, and we are unable to receive all */
    if (received == total) {
        printf("ERROR Storage full from socket");
    }

    printf("The host address is %s\n", server->h_name);
    printf("Exiting now.\n");

    /* Free the memory of http_query we allocated */
    free(http_query);

    /* Close the socket */
    close(sock);

    /* Exit the program */
    exit(0);
}
