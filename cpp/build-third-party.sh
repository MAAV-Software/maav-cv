#!/bin/bash

# Stop on errors, print commands
# See https://vaneyckt.io/posts/safer_bash_scripts_with_set_euxo_pipefail/
set -Eeuo pipefail
set -x

# Use an appropriate amount of CPU cores based on this system
cpu_cores=$(($(grep -c ^processor /proc/cpuinfo)-1))
cpu_cores=$((cpu_cores > 1 ? cpu_cores : 1))

# Compile opencv
# mkdir -p opencv/build
# cd opencv/build
# cmake ..
# make -j$cpu_cores
# cd ../..

# Install Realsense dependencies
# xorg-dev is a dependency of glfw, a dependency of realsense
# libusb provides an interface for the USB camera
sudo apt-get install -y xorg-dev libusb-1.0-0-dev

# Compile realsense
mkdir -p librealsense/build
cd librealsense/build
cmake ..
make -j$cpu_cores
