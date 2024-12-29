# OpenDNP3 Build and Run Guide

This guide explains how to build and run the OpenDNP3 master and outstation examples.

## Building OpenDNP3

1. Navigate to the project root directory:
```bash
cd opendnp3
```

2. Build and install the project:
```bash
cd cpp
cmake .
make -j
sudo make install
```

## Running the Examples

### Setup
Before running the examples, set the library path:
```bash
export LD_LIBRARY_PATH=/usr/local/lib:$LD_LIBRARY_PATH
```

### Running the Outstation
1. Navigate to the outstation example:
```bash
cd cpp/examples/outstation
```

2. Build and run:
```bash
cmake .
make
./outstation-demo
```

### Running the Master
1. Navigate to the master example:
```bash
cd cpp/examples/master
```

2. Build and run:
```bash
cmake .
make
./master-demo
```

## Packet Capture
To capture DNP3 traffic between master and outstation:
```bash
sudo tcpdump -i lo -w opendnp3.pcap port 20000
```

If you want to capture the traffic, run packet capture first, then run the outstation (server) and master (client).
