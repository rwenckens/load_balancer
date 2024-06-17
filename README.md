# Load Balancer Simulation

## Introduction

This project is a simulation of a load balancer . The load balancer manages web requests, distributes them to web servers, and incorporates DDOS protection mechanisms.

## Features

- **Load Balancer**: Manages a queue of incoming requests.
- **Web Server**: Simulates handling of requests.
- **Request Struct**: Represents network requests.
- **Dynamic Management**: Adjusts the number of web servers based on load.
- **DDOS Protection**: Blocks IP addresses that exceed request thresholds.

## Usage

To compile and run the project, use the following commands:

```sh
make && ./loadbalancer
