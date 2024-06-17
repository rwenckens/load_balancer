/**
 * @file Request.h
 * @brief Header file for the Request structure.
 */

#ifndef REQUEST_H
#define REQUEST_H

#include <string>

/**
 * @struct Request
 * @brief A structure to represent a network request.
 */
struct Request {
    std::string ipIn;  ///< Incoming IP address.
    std::string ipOut; ///< Outgoing IP address.
    int time;          ///< Processing time in clock cycles.

    /**
     * @brief Constructor for the Request structure.
     * @param in The incoming IP address.
     * @param out The outgoing IP address.
     * @param t The processing time in clock cycles.
     */
    Request(const std::string& in, const std::string& out, int t)
        : ipIn(in), ipOut(out), time(t) {}
};

#endif // REQUEST_H
