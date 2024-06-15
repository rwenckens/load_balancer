#include <iostream>
#include <vector>
#include "Request.h"
#include "Webserver.h"
#include "RequestQueue.h"
#include "Loadbalancer.h"

int main() {
    int numServers, duration;

    std::cout << "Enter the number of servers: ";
    std::cin >> numServers;
    std::cout << "Enter the duration to run the load balancer (in clock cycles): ";
    std::cin >> duration;

    Loadbalancer loadBalancer(numServers);
    
    // Generate initial requests
    loadBalancer.generateInitialRequests(numServers * 100);

    // Run the load balancer
    loadBalancer.run(duration);

    // Print status
    loadBalancer.printStatus();

    return 0;
}
