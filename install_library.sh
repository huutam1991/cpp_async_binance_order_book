#!/bin/bash

apt update
apt install g++-11 -y

# Install cmake + openssl
apt-get update -y
apt-get install cmake libssl-dev libsasl2-dev -y
cmake --version

# Install Boost
cd /
wget https://archives.boost.io/release/1.71.0/source/boost_1_71_0.tar.gz
tar xzf boost_1_71_0.tar.gz
cd /boost_1_71_0
./bootstrap.sh
./b2 install --prefix=/usr/

rm -rf /boost_1_71_0*

# Add Path
export LD_LIBRARY_PATH=/usr/local/lib:$LD_LIBRARY_PATH