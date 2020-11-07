Compile driver.cpp:

g++ -g \`pkg-config --cflags opencv4\` driver.cpp \`pkg-config --libs opencv4\` -lrealsense2 -lGL -lGLU -lpthread  -o driver

OpenCV was compiled from source using these instructions: https://linuxize.com/post/how-to-install-opencv-on-ubuntu-20-04/
