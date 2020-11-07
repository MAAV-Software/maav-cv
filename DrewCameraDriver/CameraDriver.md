# Realsense Camera Driver

## Requirements


##Compile and Run
Once all the required files are installed, run the following command:
```
g++ -I/usr/local/Cellar/opencv/4.5.0_1/include/opencv4 -std=c++11 pipeline.cpp -lrealsense2 -lopencv_core -lopencv_highgui -o pipe.exe && ./pipe.exe
```
