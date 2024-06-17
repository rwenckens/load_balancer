/**
 * @file Webserver.cpp
 * @brief Source file for the Webserver class.
 */
 
#include "Webserver.h"

Webserver::Webserver() : busy(false), remainingCycles(0) {}

bool Webserver::isBusy() const {
    return busy;
}

void Webserver::assignRequest(const Request& request) {
    busy = true;
    remainingCycles = request.time;
}

void Webserver::process() {
    if (busy) {
        remainingCycles--;
        if (remainingCycles <= 0) {
            busy = false;
        }
    }
}
