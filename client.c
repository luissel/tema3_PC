#include <stdio.h>      /* printf, sprintf */
#include <stdlib.h>     /* exit, atoi, malloc, free */
#include <unistd.h>     /* read, write, close */
#include <string.h>     /* memcpy, memset */
#include <sys/socket.h> /* socket, connect */
#include <netinet/in.h> /* struct sockaddr_in, struct sockaddr */
#include <netdb.h>      /* struct hostent, gethostbyname */
#include <arpa/inet.h>
#include <sys/types.h>
#include "parson.h"
#include "parson.c"
#include "helpers.h"
#include "requests.h"

void get_hostip (char* hostname, char *hostip) {
    char ip[100];
	struct addrinfo hints, *result;
    memset(&hints, 0, sizeof(hints));

    hints.ai_family = AF_UNSPEC;
    getaddrinfo(hostname, NULL, &hints, &result);
    inet_ntop(result->ai_family, &((struct sockaddr_in *) result->ai_addr)->sin_addr, ip, sizeof(ip));

    strcpy(hostip, ip);
    freeaddrinfo(result);
}

char *get_cookies (char *response) {
    char *cookie = calloc(1000, sizeof(char));
    char *subString = strtok(response, "\n;");

    while (subString != NULL) {
        if (strncmp(subString, "Set-Cookie", 10) == 0) {
            // adaug stringul incepand de la "Cookie" pana la ";"
            strcat(cookie, subString + 4);
            strcat(cookie, "\r\n");
        }
        subString = strtok(NULL, "\n;");
    }

    return cookie;
}

void parse_json (char *response, const char **url, const char **method,
                const char **type, const char **user, const char **pass,
                const char **token, const char **id, const char **q,
                const char **appid, const char **next_url,
                const char **next_method) {
    JSON_Value *value = json_parse_string(response);
    JSON_Object *obj = json_value_get_object(value);
    JSON_Object *data, *query_params;

    *url = json_object_get_string(obj, "url");
    *method = json_object_get_string(obj, "method");
    *type = json_object_get_string(obj, "type");

    data = json_object_get_object(obj, "data");
    query_params = json_object_get_object(data, "queryParams");

    *user = json_object_get_string(data, "username");
    *pass = json_object_get_string(data, "password");

    if (*token == NULL) {
        *token = json_object_get_string(data, "token");
    }

    *id = json_object_get_string(query_params, "id");
    *next_url = json_object_get_string(data, "url");
    *next_method = json_object_get_string(data, "method");
    *q = json_object_get_string(query_params, "q");
    *appid = json_object_get_string(query_params, "APPID");
}

int main (int argc, char *argv[]) {
    char *message, *response, *cookie, *find;
    const char *url, *method, *type, *username, *pass, *token, *id, *q, *appid,
                *next_url, *next_method;
    char res[BUFLEN];
    int sockfd, position;

    // task 1
    sockfd = open_connection(IP_SERVER, PORT_SERVER, AF_INET, SOCK_STREAM, 0);
    message = compute_get_request(IP_SERVER, "/task1/start", NULL, NULL, NULL);
    printf("\n%s\n", message);
    send_to_server(sockfd, message);
    response = receive_from_server(sockfd);
    close_connection(sockfd);

    printf("%s\n", response);
    strcpy(res, response);

    cookie = get_cookies(response);

    // positia de unde incepe jsonul
    find = strchr(res,'{');
    position = find - res;

    // extrag datele necesare din json
    parse_json(res + position, &url, &method, &type, &username, &pass, &token,
            &id, &q, &appid, &next_url, &next_method);

    // task 2
    sockfd = open_connection(IP_SERVER, PORT_SERVER, AF_INET, SOCK_STREAM, 0);
    if (strcmp(method, "POST") == 0) {
        // creez stringul de autentificare conform formatului specificat
        char auth_string[100];
        sprintf(auth_string, "username=%s&password=%s", username, pass);

        message = compute_post_request(IP_SERVER, url, auth_string, type, NULL, cookie);
        printf("\n%s\n", message);
        send_to_server(sockfd, message);
        response = receive_from_server(sockfd);
    }
    close_connection(sockfd);
    printf("%s\n", response);

    // reinitializez stringul res cu noul raspuns de la server
    memset(res, 0, BUFLEN);
    strcpy(res, response);
    cookie = get_cookies(response);

    // caut positia de unde incepe jsonul
    find = strchr(res, '{');
    position = find - res;

    // extrag datele necesare din json
    parse_json(res + position, &url, &method, &type, &username, &pass, &token,
                &id, &q, &appid, &next_url, &next_method);

    // task 3
    sockfd = open_connection(IP_SERVER, PORT_SERVER, AF_INET, SOCK_STREAM, 0);
    if (strcmp(method, "GET") == 0) {
        // formez parametrii pentru url cu raspunsurile pentru cele 2 intrebari
        // si id-ul extras din json
        char url_param[100], response1[10] = "omul", response2[10] = "numele";
        sprintf(url_param, "raspuns1=%s&raspuns2=%s&id=%s", response1 , response2, id);

        // formez mesajul cu parametrii url-ului si token-ul de autorizare
        message = compute_get_request(IP_SERVER, url, url_param, token, cookie);
        printf("\n%s\n", message);
        send_to_server(sockfd, message);
        response = receive_from_server(sockfd);
    }
    close_connection(sockfd);
    printf("%s\n", response);

    // reinitializez stringul res cu noul raspuns de la server
    memset(res, 0, BUFLEN);
    strcpy(res, response);
    cookie = get_cookies(response);

    // caut positia de unde incepe jsonul
    find = strchr(res, '{');
    position = find - res; // positia de unde incepe jsonul

    // extrag datele necesare din json
    parse_json(res + position, &url, &method, &type, &username, &pass, &token,
                &id, &q, &appid, &next_url, &next_method);

    // task 4
    sockfd = open_connection(IP_SERVER, PORT_SERVER, AF_INET, SOCK_STREAM, 0);
    if (strcmp(method, "GET") == 0) {
        message = compute_get_request(IP_SERVER, url, NULL, token, cookie);
        printf("\n%s\n", message);
        send_to_server(sockfd, message);
        response = receive_from_server(sockfd);
    }
    close_connection(sockfd);
    printf("%s\n", response);

    // reinitializez stringul res cu noul raspuns de la server
    memset(res, 0, BUFLEN);
    strcpy(res, response);
    cookie = get_cookies(response);

    // caut positia de unde incepe jsonul
    find = strchr(res, '{');
    position = find - res; // positia de unde incepe jsonul

    // extrag datele necesare din json
    parse_json(res + position, &url, &method, &type, &username, &pass, &token,
                &id, &q, &appid, &next_url, &next_method);

    if (strcmp(next_method, "GET") == 0) {
        // initializez parametrii url-ului cu datele extrase din json
        char url_params[1000];
        sprintf(url_params, "q=%s&APPID=%s", q, appid);

        char ip[100] = "", name_copy[100], *hostname;
        strcpy(name_copy, next_url);
        //extrag hostname-ul
        hostname = strtok(name_copy, "/");
        // preiau ip-ul specific hostname-ului obtinut din json
        get_hostip(hostname, ip);

        // extrag url-ul pentru a lua datele despe vreme
        char *new_url = strstr(next_url, "/");

        // deschis conexiunea pe noul ip
        sockfd = open_connection(ip, 80, AF_INET, SOCK_STREAM, 0);
        message = compute_get_request(ip, new_url, url_params, token, cookie);
        printf("\n%s\n", message);
        send_to_server(sockfd, message);
        response = receive_from_server(sockfd);
    }
    close_connection(sockfd);
    printf("%s\n", response);

    // caut positia de unde incepe jsonul
    find = strchr(response, '{');
    position = find - response;

    sockfd = open_connection(IP_SERVER, PORT_SERVER, AF_INET, SOCK_STREAM, 0);
    if (strcmp(method, "POST") == 0) {
        // trimit catre server datele despre vreme primite anterior
        message = compute_post_request(IP_SERVER, url, response + position, type, token, cookie);
        printf("\n%s\n", message);
        send_to_server(sockfd, message);
        response = receive_from_server(sockfd);
    }
    close_connection(sockfd);
    printf("%s\n", response);

    free(message);
    free(cookie);
    return 0;
}