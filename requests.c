#include <stdlib.h>     /* exit, atoi, malloc, free */
#include <stdio.h>
#include <unistd.h>     /* read, write, close */
#include <string.h>     /* memcpy, memset */
#include <sys/socket.h> /* socket, connect */
#include <netinet/in.h> /* struct sockaddr_in, struct sockaddr */
#include <netdb.h>      /* struct hostent, gethostbyname */
#include <arpa/inet.h>
#include "helpers.h"
#include "requests.h"

char *compute_get_request(char *host, const char *url, char *url_params,
                             const char * token, char *cookie) {
    char *message = calloc(BUFLEN, sizeof(char));
    char *line = calloc(LINELEN, sizeof(char));

    // Scriu numele metodei, calea, parametri din url (daca exista) si tipul protocolului
    if (url_params != NULL) {
        sprintf(line, "GET %s?%s HTTP/1.1", url, url_params);
    } else {
        sprintf(line, "GET %s HTTP/1.1", url);
    }
    compute_message(message, line);

    // Adaug host-ul
    sprintf(line, "HOST: %s", host);
    compute_message(message, line);

    // Adaug token-ul sau cookies, respectand forma protocolului
    if (token != NULL) {
        sprintf(line, "Authorization: Bearer %s", token);
        compute_message(message, line);
    }

    if (cookie != NULL) {
        strcat(message,cookie);
    }

    // Adaug linia de final
    compute_message(message, "");

    return message;
}

char *compute_post_request(char *host, const char *url, char *form_data, const char *type, const char *token, char *cookie) {
    char *message = calloc(BUFLEN, sizeof(char));
    char *line = calloc(LINELEN, sizeof(char));

    // Scriu numele metodei, calea si tipul protocolului
    sprintf(line, "POST %s HTTP/1.1", url);
    compute_message(message, line);


    // Adaug host-ul
    sprintf(line, "HOST: %s", host);
    compute_message(message, line);

    // Adaug headerele Content-Type si Content-Length
    sprintf(line, "Content-Type: %s", type);
    compute_message(message, line);

    sprintf(line, "Content-Length: %ld", strlen(form_data));
    compute_message(message, line);

    // Adaug token-ul sau cookies, respectand forma protocolului
    if (token != NULL) {
        sprintf(line, "Authorization: Bearer %s", token);
        compute_message(message, line);
    }

    if (cookie != NULL) {
        strcat(message,cookie);
    }
    // Adaug linia de final de antent
    compute_message(message, "");

    // Adaug data
    sprintf(line, "%s", form_data);
    compute_message(message, line);

    return message;
}