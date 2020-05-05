#ifndef _REQUESTS_
#define _REQUESTS_

char *compute_get_request(char *host, const char *url, char *url_params, const char *token, char *cookie);
char *compute_post_request(char *host, const char *url, char *form_data, const char *type, const char *token, char *cookie);

#endif