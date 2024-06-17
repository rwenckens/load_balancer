#include "RequestQueue.h"

void RequestQueue::addRequest(const Request& request) {
    requests.push(request);
}

bool RequestQueue::hasRequest() const {
    return !requests.empty();
}

size_t RequestQueue::size() const {
    return requests.size();
}

Request RequestQueue::getNextRequest() {
    if (hasRequest()) {
        Request request = requests.front();
        requests.pop();
        return request;
    }
    return Request("", "", 0);
}
