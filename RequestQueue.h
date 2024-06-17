/**
 * @file RequestQueue.h
 * @brief Header file for the RequestQueue class.
 */

#ifndef REQUESTQUEUE_H
#define REQUESTQUEUE_H

#include <queue>
#include "Request.h"

/**
 * @class RequestQueue
 * @brief A class to manage a queue of network requests.
 */
class RequestQueue {
private:
    std::queue<Request> requests; ///< Queue to store requests.

public:
    /**
     * @brief Adds a request to the queue.
     * @param request The request to be added.
     */
    void addRequest(const Request& request);

    /**
     * @brief Checks if the queue has any requests.
     * @return True if the queue is not empty, false otherwise.
     */
    bool hasRequest() const;

    /**
     * @brief Gets the size of the queue.
     * @return The number of requests in the queue.
     */
    size_t size() const;

    /**
     * @brief Retrieves and removes the next request from the queue.
     * @return The next request in the queue. If the queue is empty, returns a default request.
     */
    Request getNextRequest();
};

#endif // REQUESTQUEUE_H
