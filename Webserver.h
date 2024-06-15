#ifndef WEBSERVER_H
#define WEBSERVER_H

#include "Request.h"
#include <chrono>
#include <iostream>

using std::cout, std::endl;

class Webserver {
private:
    bool busy;
    std::chrono::steady_clock::time_point endTime;

public:
    Webserver() : busy(false) {}

    bool isBusy() const { return busy; }

    void assignRequest(const Request& request) {
        busy = true;
        endTime = std::chrono::steady_clock::now() + std::chrono::seconds(request.time);
    }

    void process() {
        if (busy && std::chrono::steady_clock::now() >= endTime) {
            busy = false;
        }
    }
};

#endif // WEBSERVER_H
