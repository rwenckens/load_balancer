#ifndef REQUEST_H
#define REQUEST_H

#include <string>

struct Request {
    std::string ipIn;
    std::string ipOut;
    int time;

    Request(const std::string& in, const std::string& out, int t)
        : ipIn(in), ipOut(out), time(t) {}
};

#endif 
