#ifndef REQUESTQUEUE_H
#define REQUESTQUEUE_H

#include <queue>
#include "Request.h"

class RequestQueue {
private:
    std::queue<Request> requests;

public:
    void addRequest(const Request& request) {
        requests.push(request);
    }

    bool hasRequest() const {
        return !requests.empty();
    }

    size_t size() const {
        return requests.size();
    }

    Request getNextRequest() {
        if (hasRequest()) {
            Request request = requests.front();
            requests.pop();
            return request;
        }
        return Request("", "", 0);
    }
};

#endif // REQUESTQUEUE_H
