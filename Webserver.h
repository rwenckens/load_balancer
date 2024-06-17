/**
 * @file Webserver.h
 * @brief Header file for the Webserver class.
 */

#ifndef WEBSERVER_H
#define WEBSERVER_H

#include "Request.h"
#include <iostream>

/**
 * @class Webserver
 * @brief A class to simulate a web server processing network requests.
 */
class Webserver {
private:
    bool busy; ///< Flag indicating if the server is busy.
    int remainingCycles; ///< Number of clock cycles remaining to process the current request.

public:
    /**
     * @brief Default constructor for the Webserver class.
     */
    Webserver();

    /**
     * @brief Checks if the server is busy.
     * @return True if the server is busy, false otherwise.
     */
    bool isBusy() const;

    /**
     * @brief Assigns a request to the server.
     * @param request The request to be assigned.
     */
    void assignRequest(const Request& request);

    /**
     * @brief Processes the current request, decrementing the remaining clock cycles.
     */
    void process();
};

#endif
