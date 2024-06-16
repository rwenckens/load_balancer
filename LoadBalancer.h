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

using std::cout, std::endl;

class Loadbalancer {
private:
    std::vector<Webserver> webservers;
    RequestQueue requestQueue;
    int clockCycle;
    std::ofstream logFile;

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
                    // cout << "Removed an idle webserver. Total servers: " << webservers.size() << endl;
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

        cout << "--- Ensuring balance busyRatio = " << busyRatio << " pending requests = " << pendingRequests << endl;
        logFile << "--- Ensuring balance busyRatio = " << busyRatio << " pending requests = " << pendingRequests << endl;

        if (busyRatio > 0.75 && pendingRequests > webservers.size() * 2) {
            int currSize = webservers.size();
            for (int i = 0; i < currSize * 2; i++) {
                webservers.emplace_back();
            }
            cout << "--- Added webservers. Total servers: " << webservers.size() << endl;
            logFile << "--- Added webservers. Total servers: " << webservers.size() << endl;

        } else if (busyRatio < 0.4 && webservers.size() > 1) {
            int currSize = webservers.size();
            for (int i = 0; i < currSize / 2; i++) {
                removeIdle();
            }
            cout << "--- Removed webservers. Total servers: " << webservers.size() << endl;
            logFile << "--- Removed webservers. Total servers: " << webservers.size() << endl;
        }
    }

public:
    Loadbalancer(int numServers)
        : webservers(numServers), clockCycle(0), logFile("log.txt") {}

    ~Loadbalancer() {
        if (logFile.is_open()) {
            logFile.close();
        }
    }

    void generateInitialRequests(int numRequests) {
        for (int i = 0; i < numRequests; ++i) {
            requestQueue.addRequest(Request(generateRandomIP(), generateRandomIP(), generateRandomTime()));
        }
        cout << numRequests << " initial requests added to the initial queue" << endl;
        logFile << numRequests << " initial requests added to the initial queue" << endl;

    }

void run(int maxClockCycles) {
    cout << "Load Balancer has started running" << endl;
    logFile << "Load Balancer has started running" << endl;


    auto start = std::chrono::steady_clock::now();
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(1, 100); 

    while (clockCycle < maxClockCycles) {
        for (int i = 0; i < webservers.size(); i++) {
            clockCycle++;
            if (!webservers[i].isBusy() && requestQueue.hasRequest()) {
                Request next = requestQueue.getNextRequest();
                webservers[i].assignRequest(next);
                cout << "Request with ipIn: " << next.ipIn << " ipOut: " << next.ipOut << " time: " << next.time << " has been assigned to webserver " << i << " at clock cycle " << clockCycle << endl;
                logFile << "Request with ipIn: " << next.ipIn << " ipOut: " << next.ipOut << " time: " << next.time << " has been assigned to webserver " << i << " at clock cycle " << clockCycle << endl;

            }
            webservers[i].process();
        }

        int random = dis(gen);
        if (random % 3 == 0) {
            Request randomRequest(generateRandomIP(), generateRandomIP(), generateRandomTime());
            requestQueue.addRequest(randomRequest);
            cout << "Random request with ipIn: " << randomRequest.ipIn << " ipOut: " << randomRequest.ipOut << " time: " << randomRequest.time << " has been added to the request queue" << endl;
            logFile << "Random request with ipIn: " << randomRequest.ipIn << " ipOut: " << randomRequest.ipOut << " time: " << randomRequest.time << " has been added to the request queue" << endl;

        }

        maintainBalance();

    }
}


    void printStatus() const {
        // cout << "Load Balancer Status after " << clockCycle << " clock cycles:\n";
        // for (size_t i = 0; i < webservers.size(); ++i) {
        //     cout << "Webserver " << i + 1 << " is " << (webservers[i].isBusy() ? "busy\n" : "idle\n");
        // }
    }
};

#endif 
