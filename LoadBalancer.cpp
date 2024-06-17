/**
 * @file Loadbalancer.cpp
 * @brief Source file for the Loadbalancer class implementation.
 */

#include "Loadbalancer.h"
#include <iostream>

using std::endl;

std::string Loadbalancer::generateRandomIP() {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(0, 255);

    return std::to_string(dis(gen)) + "." + std::to_string(dis(gen)) + "." +
           std::to_string(dis(gen)) + "." + std::to_string(dis(gen));
}

int Loadbalancer::generateRandomTime() {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(1, 10);
    return dis(gen);
}

void Loadbalancer::removeIdle() {
    int countRemoved = 0;
    size_t toRemove = webservers.size() / 2;
    for (size_t i = 0; i < toRemove; ++i) {
        if (!webservers[i].isBusy()) {
            webservers.erase(webservers.begin() + i);
            countRemoved++;
        }
    }
}

void Loadbalancer::maintainBalance() {
    int busyServers = 0;
    for (size_t i = 0; i < webservers.size(); ++i) {
        if (webservers[i].isBusy()) {
            busyServers++;
        }
    }

    double busyRatio = static_cast<double>(busyServers) / webservers.size();
    int pendingRequests = requestQueue.size();

    logFile << "--- Ensuring balance busyRatio = " << busyRatio << " pending requests = " << pendingRequests << endl;

    if (busyRatio > 0.75 && pendingRequests > int(webservers.size() * 2)) {
        int currSize = webservers.size();
        for (int i = 0; i < currSize * 2; i++) {
            webservers.emplace_back();
        }
        logFile << "--- Added webservers. Total servers: " << webservers.size() << endl;
    } else if (busyRatio < 0.4 && webservers.size() > 1) {
        removeIdle();
        logFile << "--- Removed webservers. Total servers: " << webservers.size() << endl;
    }
}

bool Loadbalancer::isBlocked(const std::string& ip) {
    return firewallEnabled && (ipRequestCount[ip] > requestThreshold);
}

void Loadbalancer::blockIP(const std::string& ip) {
    logFile << "IP " << ip << " has been blocked due to suspected DOS attack." << endl;
    ipRequestCount[ip] = ++requestThreshold; 
}

Loadbalancer::Loadbalancer(int numServers, bool firewall, int maxReqByIP)
    : webservers(numServers), clockCycle(0), logFile("log.txt"), firewallEnabled(firewall), requestThreshold(maxReqByIP) {}

Loadbalancer::~Loadbalancer() {
    if (logFile.is_open()) {
        logFile.close();
    }
}

void Loadbalancer::generateInitialRequests(int numRequests) {
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
    logFile << numRequests << " initial requests added to the initial queue" << endl;
}

void Loadbalancer::run(int maxClockCycles) {
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
            logFile << "Random request with ipIn: " << randomRequest.ipIn << " ipOut: " << randomRequest.ipOut << " time: " << randomRequest.time << " has been added to the request queue" << endl;
        }

        maintainBalance();
    }
}

void Loadbalancer::simulateAttack(int numRequests, const std::string& attackIP) {
    int countAttackReq = 0;
    for (int i = 0; i < numRequests; ++i) {
        Request attackRequest(attackIP, generateRandomIP(), generateRandomTime());
        taskTimes.push_back(attackRequest.time);
        requestQueue.addRequest(attackRequest);
        countAttackReq++;
    }
    logFile << countAttackReq << " ATTACK requests added to initial queue for DDOS simulation " << endl;
}

void Loadbalancer::enableFirewall() {
    firewallEnabled = true;
    logFile << "Firewall has been enabled." << endl;
}

void Loadbalancer::disableFirewall() {
    firewallEnabled = false;
    logFile << "Firewall has been disabled." << endl;
}

void Loadbalancer::printStatus(int numServers, int startingQueueSize, int clockCycles) {
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
