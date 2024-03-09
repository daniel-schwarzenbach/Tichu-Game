//
// Created by Daniel on Spring 2023 inspired by Manuel
//

#ifndef TICHU_REQUEST_HANDLER_H
#define TICHU_REQUEST_HANDLER_H

#include "../common/network/responses/server_response.h"
#include "../common/network/requests/client_request.h"
#include "../common/network/responses/request_response.h"

class RequestHandler {
public:
    static request_response* handle_request(
            const client_request* const req
    );
};
#endif //RequestHandler_REQUEST_HANDLER_H
