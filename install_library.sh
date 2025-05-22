#!/bin/bash

apt update -y
apt install -y build-essential

# Install cmake + openssl
apt-get update -y
apt-get install cmake libssl-dev libsasl2-dev -y
cmake --version

# Install Boost
apt update -y
apt install -y libboost-all-dev

# Add Path
export LD_LIBRARY_PATH=/usr/local/lib:$LD_LIBRARY_PATH