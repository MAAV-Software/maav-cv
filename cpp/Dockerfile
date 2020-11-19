# Michigan Autonomous Aerial Vehicles
# Dockerfile for environment to test camera driver implementations in C++
# Look at README.md for information on building the image, running the container

FROM ubuntu:18.04

RUN apt-get -y update && \
    apt-get -y install git cmake build-essential

# Install OpenCV version 4.4.0
# https://docs.opencv.org/4.4.0/d7/d9f/tutorial_linux_install.html
WORKDIR /tmp
RUN git clone --depth 1 --branch 4.4.0 \
    https://github.com/opencv/opencv.git
WORKDIR /tmp/opencv
RUN mkdir build && \
    cd build && \
    cmake .. && \
    make -j$(expr $(nproc) - 1) && \
    make install

# Install Librealsense version 2.40.0
# https://github.com/IntelRealSense/librealsense/blob/master/doc/installation.md
RUN apt-get -y update && \
    apt-get -y install libglfw3-dev libgl1-mesa-dev libglu1-mesa-dev at \
    libssl-dev libusb-1.0-0-dev pkg-config libgtk-3-dev
WORKDIR /tmp
RUN git clone --depth 1 --branch v2.40.0 \
    https://github.com/IntelRealSense/librealsense.git
WORKDIR /tmp/librealsense
RUN mkdir build && \
    cd build && \
    cmake .. && \
    make -j$(expr $(nproc) - 1) && \
    make install

# Cleanup
RUN apt-get autoclean && apt-get clean
RUN rm -rf /var/lib/apt/lists/* /tmp/* /var/tmp/*

VOLUME ["/source"]
WORKDIR /source
CMD ["bash"]

# docker run \
#     -v .:/source \
#     -it --rm \
#     --name maav-cv maav-camera-driver
