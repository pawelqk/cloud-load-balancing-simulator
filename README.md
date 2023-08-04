# cloud-load-balancing-simulator

Master thesis project used to test multiple algorithms to solve a scheduling problem, both in offline and online versions. Following algorithms are implemented:
- Random
- Round robin
- First In First Out
- Shortest Remaining Processing Time
- Shortest Elapsed Time First
- Longest Remaining Time First
- Simulated Annealing
- Artificial bee colony
- Genetic algorithm

## Prerequisites
CMake >= 3.1

## Installation
```shell
cmake . && make
```
## Running
```shell
./bin/cloud-load-balancing-simulator <flags>
```
Available flags:
* -d -- turn logging debug flags on
* --stdout - turn logging on stdout
* --files - turn logging to `logs/` directory

## Running tests
```shell
./bin/cloud-load-balancing-simulator-test
```
