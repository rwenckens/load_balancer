#ifndef LOADBALANCER_H
#define LOADBALANCER_H

#include <thread>
#include <vector>
#include <random>
#include <iostream>
#include <chrono>
#include <unordered_map>
#include "Webserver.h"
#include <fstream>
#include "RequestQueue.h"

using std::cout; 
using std::endl;

class Loadbalancer {
private:
    std::vector<Webserver> webservers;
    RequestQueue requestQueue;
    int clockCycle;
    std::ofstream logFile;

    //! stats
    std::vector<int> taskTimes;

    //! DDOS PROTECTION
    std::unordered_map<std::string, int> ipRequestCount; 
    bool firewallEnabled;
    int requestThreshold;
    

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
        int countRemoved = 0;
        size_t toRemove = webservers.size() / 2;
        for (size_t i = 0; i < toRemove; ++i) {
            if (!webservers[i].isBusy()) {
                webservers.erase(webservers.begin() + i);
                countRemoved++;
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

        // cout << "--- Ensuring balance busyRatio = " << busyRatio << " pending requests = " << pendingRequests << endl;
        logFile << "--- Ensuring balance busyRatio = " << busyRatio << " pending requests = " << pendingRequests << endl;

        if (busyRatio > 0.75 && pendingRequests > int(webservers.size() * 2)) {
            int currSize = webservers.size();
            for (int i = 0; i < currSize * 2; i++) {
                webservers.emplace_back();
            }
            // cout << "--- Added webservers. Total servers: " << webservers.size() << endl;
            logFile << "--- Added webservers. Total servers: " << webservers.size() << endl;

        } else if (busyRatio < 0.4 && webservers.size() > 1) {
            removeIdle();
            // cout << "--- Removed webservers. Total servers: " << webservers.size() << endl;
            logFile << "--- Removed webservers. Total servers: " << webservers.size() << endl;
        }
    }

    bool isBlocked(const std::string& ip) {
        return firewallEnabled && (ipRequestCount[ip] > requestThreshold);
    }

    void blockIP(const std::string& ip) {
        // cout << "IP " << ip << " has been blocked due to suspected DOS attack." << endl;
        logFile << "IP " << ip << " has been blocked due to suspected DOS attack." << endl;
        ipRequestCount[ip] = ++requestThreshold; 
    }

public:
Loadbalancer(int numServers, bool firewall = false, int maxReqByIP = 100)
        : webservers(numServers), clockCycle(0), logFile("log.txt"), firewallEnabled(firewall), requestThreshold(maxReqByIP) {}

    ~Loadbalancer() {
        if (logFile.is_open()) {
            logFile.close();
        }
    }

    void generateInitialRequests(int numRequests) {
        for (int i = 0; i < numRequests; ++i) {
            Request req(generateRandomIP(), generateRandomIP(), generateRandomTime());
            taskTimes.push_back(req.time);
            if (firewallEnabled) {
                ipRequestCount[req.ipIn]++;
                if (isBlocked(req.ipIn)) {
                    blockIP(req.ipIn);
                    continue; 
                }
            }
            requestQueue.addRequest(req);
        }
        // cout << numRequests << " initial requests added to the initial queue" << endl;
        logFile << numRequests << " initial requests added to the initial queue" << endl;
    }


    void run(int maxClockCycles) {
        // cout << "Load Balancer has started running" << endl;
        logFile << "Load Balancer has started running" << endl;

        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_int_distribution<> dis(1, 100); 

        while (clockCycle < maxClockCycles) {
            for (size_t i = 0; i < webservers.size(); i++) {
                clockCycle++;
                if (!webservers[i].isBusy() && requestQueue.hasRequest()) {
                    Request next = requestQueue.getNextRequest();

                    if (firewallEnabled) {
                        ipRequestCount[next.ipIn]++;
                        if (isBlocked(next.ipIn)) {
                            blockIP(next.ipIn);
                            continue; 
                        }
                    }
                    webservers[i].assignRequest(next);
                    cout << "Request with ipIn: " << next.ipIn << " ipOut: " << next.ipOut << " time: " << next.time << " has been assigned to webserver " << i << " at clock cycle " << clockCycle << endl;
                    logFile << "Request with ipIn: " << next.ipIn << " ipOut: " << next.ipOut << " time: " << next.time << " has been assigned to webserver " << i << " at clock cycle " << clockCycle << endl;

                }
                webservers[i].process();
            }

            int random = dis(gen);
            if (random % 2 == 0) {
                Request randomRequest(generateRandomIP(), generateRandomIP(), generateRandomTime());
                taskTimes.push_back(randomRequest.time);
                if (firewallEnabled) {
                    ipRequestCount[randomRequest.ipIn]++;
                    if (isBlocked(randomRequest.ipIn)) {
                        blockIP(randomRequest.ipIn);
                        continue; 
                    }
                }

                requestQueue.addRequest(randomRequest);
                // cout << "Random request with ipIn: " << randomRequest.ipIn << " ipOut: " << randomRequest.ipOut << " time: " << randomRequest.time << " has been added to the request queue" << endl;
                logFile << "Random request with ipIn: " << randomRequest.ipIn << " ipOut: " << randomRequest.ipOut << " time: " << randomRequest.time << " has been added to the request queue" << endl;
            }

            maintainBalance();
        }
    }

    void simulateAttack(int numRequests, const std::string& attackIP) {
        int countAttackReq = 0;
        for (int i = 0; i < numRequests; ++i) {
            Request attackRequest(attackIP, generateRandomIP(), generateRandomTime());
            taskTimes.push_back(attackRequest.time);
            requestQueue.addRequest(attackRequest);
            countAttackReq++;
        }

        // cout << countAttackReq << " ATTACK requests added to initial queue for DDOS simulation " << endl;
        logFile << countAttackReq << " ATTACK requests added to initial queue for DDOS simulation " << endl;
    }

    void enableFirewall() {
        firewallEnabled = true;
        // cout << "Firewall has been enabled." << endl;
        logFile << "Firewall has been enabled." << endl;
    }

    void disableFirewall() {
        firewallEnabled = false;
        // cout << "Firewall has been disabled." << endl;
        logFile << "Firewall has been disabled." << endl;
    }


    void printStatus(int numServers, int startingQueueSize, int clockCycles) {
        int minTaskTime = taskTimes.at(0);
        int maxTaskTime = taskTimes.at(0);
        int sum = 0;
        for (const auto time : taskTimes) {
            if (time < minTaskTime) minTaskTime = time;
            if (time > maxTaskTime) maxTaskTime = time;
            sum += time;
        }
        double averageTaskTime = sum / (double) taskTimes.size();

        logFile << "------------------------" << endl << endl << endl;
        logFile << "servers = " << numServers << endl;
        logFile << "clock cycles = " << clockCycles << endl;
        logFile << "starting queue size = " << startingQueueSize << endl;
        logFile << "ending queue size = " << requestQueue.size() << endl;
        logFile << "range for task times = " << minTaskTime << " to " << maxTaskTime << endl;
        logFile << "average task time = " << averageTaskTime << endl;
    }
};

#endif 
