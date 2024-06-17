#ifndef WEBSERVER_H
#define WEBSERVER_H

#include "Request.h"
#include <iostream>

class Webserver {
private:
    bool busy;
    int remainingCycles;

public:
    Webserver() : busy(false), remainingCycles(0) {}

    bool isBusy() const { return busy; }

    void assignRequest(const Request& request) {
        busy = true;
        remainingCycles = request.time;
    }

    void process() {
        if (busy) {
            remainingCycles--;
            if (remainingCycles <= 0) {
                busy = false;
            }
        }
    }
};

#endif 
