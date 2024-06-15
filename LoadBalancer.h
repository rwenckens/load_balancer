#ifndef LOADBALANCER_H
#define LOADBALANCER_H

#include <thread>
#include <vector>
#include <random>
#include <iostream>
#include <chrono>
#include "Webserver.h"
#include "RequestQueue.h"

using std::cout, std::endl;

class Loadbalancer {
private:
    std::vector<Webserver> webservers;
    RequestQueue requestQueue;
    int clockCycle;

    std::string generateRandomIP() {
        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_int_distribution<> dis(0, 255);

        return std::to_string(dis(gen)) + "." + std::to_string(dis(gen)) + "." +
               std::to_string(dis(gen)) + "." + std::to_string(dis(gen));
    }

    int generateRandomTime() {
        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_int_distribution<> dis(1, 10);
        return dis(gen);
    }

    void removeIdle() {
        for (size_t i = 0; i < webservers.size(); ++i) {
                if (!webservers[i].isBusy()) {
                    webservers.erase(webservers.begin() + i);
                    // std::cout << "Removed an idle webserver. Total servers: " << webservers.size() << std::endl;
                    break;
                }
            }
    }

    void maintainBalance() {
        int busyServers = 0;
        for (size_t i = 0; i < webservers.size(); ++i) {
            if (webservers[i].isBusy()) {
                busyServers++;
            }
        }

        double busyRatio = static_cast<double>(busyServers) / webservers.size();
        int pendingRequests = requestQueue.size();

        // Adjust the number of servers based on busy ratio and pending requests
        cout << "--- Ensuring balance busyRatio = " << busyRatio << " pending requests = " << pendingRequests << endl;

        if (busyRatio > 0.75 && pendingRequests > webservers.size() * 2) {
            // Increase the number of servers if more than 75% are busy and pending requests are more than twice the number of servers
            int currSize = webservers.size();
            for (int i = 0; i < currSize * 2; i++) {
                webservers.emplace_back();
            }
            std::cout << "--- Added webservers. Total servers: " << webservers.size() << std::endl;
        } else if (busyRatio < 0.4 && webservers.size() > 1) {
            // Find an idle server to remove
            int currSize = webservers.size();
            for (int i = 0; i < currSize / 2; i++) {
                removeIdle();
            }
            std::cout << "--- Removed webservers. Total servers: " << webservers.size() << std::endl;

        }
    }

public:
    Loadbalancer(int numServers)
        : webservers(numServers), clockCycle(0) {}

    void generateInitialRequests(int numRequests) {
        for (int i = 0; i < numRequests; ++i) {
            requestQueue.addRequest(Request(generateRandomIP(), generateRandomIP(), generateRandomTime()));
        }
        cout << numRequests << " initial requests added to the initial queue" << endl;
    }

void run(int duration) {
    auto start = std::chrono::steady_clock::now();
    std::cout << "Load Balancer has started running" << std::endl;

    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(1, 100); 

    while (std::chrono::duration_cast<std::chrono::seconds>(std::chrono::steady_clock::now() - start).count() < duration) {
        clockCycle++;

        for (int i = 0; i < webservers.size(); i++) {
            if (!webservers[i].isBusy() && requestQueue.hasRequest()) {
                Request next = requestQueue.getNextRequest();
                webservers[i].assignRequest(next);
                std::cout << "Request with ipIn: " << next.ipIn << " ipOut: " << next.ipOut << " time: " << next.time
                          << " has been assigned to webserver " << i << std::endl;
            }
            webservers[i].process();
        }

        int random = dis(gen);
        if (random % 3 == 0) {
            Request randomRequest(generateRandomIP(), generateRandomIP(), generateRandomTime());
            requestQueue.addRequest(randomRequest);
            std::cout << "Random request with ipIn: " << randomRequest.ipIn << " ipOut: " << randomRequest.ipOut
                      << " time: " << randomRequest.time << " has been added to the request queue" << std::endl;
        }

        maintainBalance();

        std::this_thread::sleep_for(std::chrono::seconds(1));  // Simulate each clock cycle as 1 second
    }
}


    void printStatus() const {
        // std::cout << "Load Balancer Status after " << clockCycle << " clock cycles:\n";
        // for (size_t i = 0; i < webservers.size(); ++i) {
        //     std::cout << "Webserver " << i + 1 << " is " << (webservers[i].isBusy() ? "busy\n" : "idle\n");
        // }
    }
};

#endif 
