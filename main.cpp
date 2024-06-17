/**
 * @file main.cpp
 * @brief Entry point for the load balancer simulation program.
 */

#include <iostream>
#include "Request.h"
#include "Webserver.h"
#include "RequestQueue.h"
#include "Loadbalancer.h"

/**
 * @brief Main function to run the load balancer simulation.
 * 
 * The program prompts the user for various inputs, initializes the load balancer,
 * and runs the simulation for a specified number of clock cycles.
 * 
 * @return int Exit status of the program.
 */
int main() {
    int numServers, maxClockCycles;
    bool firewallEnabled;
    int attackRequests;
    std::string attackIP;
    int maxReqByIp;
    int startingQueueSize;

    std::cout << "Enter the number of servers: ";
    std::cin >> numServers;
    std::cout << "Enter the maximum number of clock cycles to run the load balancer: ";
    std::cin >> maxClockCycles;
    std::cout << "Enable firewall for DDOS protection? (1 for yes, 0 for no): ";
    std::cin >> firewallEnabled;

    if (firewallEnabled) {
        std::cout << "Max total requests by a specific IP before blocked? ";
        std::cin >> maxReqByIp;
        std::cout << "Enter the number of requests to simulate an attack: ";
        std::cin >> attackRequests;
        std::cout << "Enter the IP address to simulate the attack from: ";
        std::cin >> attackIP;
    }
    
    std::cout << "Enter starting queue size (usually numServers * 100): ";
    std::cin >> startingQueueSize;

    Loadbalancer loadBalancer(numServers, firewallEnabled, maxReqByIp);
    loadBalancer.generateInitialRequests(startingQueueSize);
    if (firewallEnabled) {
        loadBalancer.simulateAttack(attackRequests, attackIP);
    }
    loadBalancer.run(maxClockCycles);
    loadBalancer.printStatus(numServers, startingQueueSize, maxClockCycles);
    
    return 0;
}
