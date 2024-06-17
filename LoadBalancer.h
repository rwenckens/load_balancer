/**
 * @file Loadbalancer.h
 * @brief Header file for the Loadbalancer class.
 */

#ifndef LOADBALANCER_H
#define LOADBALANCER_H

#include <thread>
#include <vector>
#include <random>
#include <iostream>
#include <chrono>
#include "Webserver.h"
#include <fstream>
#include "RequestQueue.h"
#include <unordered_map>

/**
 * @class Loadbalancer
 * @brief A class to simulate a load balancer with DDOS protection.
 */
class Loadbalancer {
private:
    std::vector<Webserver> webservers; ///< Vector of webservers.
    RequestQueue requestQueue; ///< Queue to manage requests.
    int clockCycle; ///< Clock cycle counter.
    std::ofstream logFile; ///< Log file for recording events.

    std::vector<int> taskTimes; ///< Vector to store task times.

    //! DDOS PROTECTION
    std::unordered_map<std::string, int> ipRequestCount; ///< Map to track request count per IP.
    bool firewallEnabled; ///< Flag to enable/disable firewall.
    int requestThreshold; ///< Maximum allowed requests per IP before blocking.

    /**
     * @brief Generates a random IP address.
     * @return A randomly generated IP address as a string.
     */
    std::string generateRandomIP();

    /**
     * @brief Generates a random processing time for a request.
     * @return A random time in clock cycles.
     */
    int generateRandomTime();

    /**
     * @brief Removes idle webservers to maintain balance.
     */
    void removeIdle();

    /**
     * @brief Maintains balance of webservers based on load.
     */
    void maintainBalance();

    /**
     * @brief Checks if an IP is blocked due to exceeding the request threshold.
     * @param ip The IP address to check.
     * @return True if the IP is blocked, false otherwise.
     */
    bool isBlocked(const std::string& ip);

    /**
     * @brief Blocks an IP address.
     * @param ip The IP address to block.
     */
    void blockIP(const std::string& ip);

public:
    /**
     * @brief Constructor for the Loadbalancer class.
     * @param numServers Number of initial webservers.
     * @param firewall Flag to enable/disable firewall.
     * @param maxReqByIP Maximum allowed requests per IP before blocking.
     */
    Loadbalancer(int numServers, bool firewall = false, int maxReqByIP = 100);

    /**
     * @brief Destructor for the Loadbalancer class.
     */
    ~Loadbalancer();

    /**
     * @brief Generates initial requests and adds them to the queue.
     * @param numRequests Number of initial requests to generate.
     */
    void generateInitialRequests(int numRequests);

    /**
     * @brief Runs the load balancer for a specified number of clock cycles.
     * @param maxClockCycles Maximum number of clock cycles to run.
     */
    void run(int maxClockCycles);

    /**
     * @brief Simulates a DDOS attack by generating multiple requests from the same IP.
     * @param numRequests Number of attack requests to generate.
     * @param attackIP The IP address to simulate the attack from.
     */
    void simulateAttack(int numRequests, const std::string& attackIP);

    /**
     * @brief Enables the firewall.
     */
    void enableFirewall();

    /**
     * @brief Disables the firewall.
     */
    void disableFirewall();

    /**
     * @brief Prints the status of the load balancer.
     * @param numServers Number of webservers.
     * @param startingQueueSize Initial size of the request queue.
     * @param clockCycles Number of clock cycles the load balancer has run.
     */
    void printStatus(int numServers, int startingQueueSize, int clockCycles);
};

#endif // LOADBALANCER_H
