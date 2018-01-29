//
// Created by dxing97 on 1/15/18.
//

#include <curl/curl.h>
#include <ulfius.h>
#include "libdiscord.h"

#ifndef LIBDISCORD_REST_H
#define LIBDISCORD_REST_H

/*
 * functions related to the REST API
 *
 */

/*
 * HTTP verb enums.
 */
enum ld_rest_http_verb {
    LD_REST_VERB_GET,
    LD_REST_VERB_PUT,
    LD_REST_VERB_POST,
    LD_REST_VERB_PATCH,
    LD_REST_VERB_DELETE
};

/*
 * rather opaque structure used to process headers
 * utilizes ulfius' _u_map API
 */
struct ld_headers {
    struct _u_map *umap;
};

/*
 * http request fields
 */
struct ld_rest_request {
    enum ld_rest_http_verb verb;
    char *base_url;
    char *endpoint;
    char *body;
    size_t body_size;
    struct _u_map *headers;
    int timeout; //ulfius default is 0
};



struct ld_rest_response {
    long http_status;
    struct _u_map *headers;
    void *body;
    size_t body_length;
};

/*
 * allocates memory
 * initializes a request with defaults
 */
struct ld_rest_request * ld_rest_init_request();

/*
 * allocates memory
 * initializes a response with defaults
 */
struct ld_rest_response * ld_rest_init_response();

/*
 * frees memory for request
 */
int ld_rest_free_request(struct ld_rest_request *request);

/*
 * frees memory for response
 */
int ld_rest_free_response(struct ld_rest_response *response);


/*
 * takes a request, performs it, then saves the response
 * returns 0 on successful request (even if the response is 4XX)
 * rather slow, since it creates and destroys a curl handle.
 * basically a wrapper around ulfius
 */
int ld_rest_send_blocking_request(struct ld_rest_request *request, struct ld_rest_response *response);

/*
 * makes a HTTP request of some kind to some URL with some headers
 * blocking
 */
int _ld_rest_blocking_request(
        enum ld_rest_http_verb verb,
        char *url,
        struct curl_slist headers,
        char *user_agent
);

/*
 * takes a verb enum and returns a read-only string for that verb. DO NOT MODIFY THE RETURNED VALUE, it is read-only
 */
char *ld_rest_verb_enum2str(enum ld_rest_http_verb verb);

/*
 * generates ld_rest_request for GET /gateway
 */
struct ld_rest_request *ld_get_gateway();

/*
 * generates ld_rest_request for GET /gateway/bot
 */
struct ld_rest_request *ld_get_gateway_bot();
int ld_post_channel_message();

#endif //LIBDISCORD_REST_H