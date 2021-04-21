# cloud-load-balancing-simulator

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
* --files - turn logging to logs/ directory

## Running tests
```shell
./bin/cloud-load-balancing-simulator-test
```
