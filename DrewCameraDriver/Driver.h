#include <opencv2/opencv.hpp>
#include <opencv2/highgui.hpp>
#include <librealsense2/rs.hpp>
#include <iostream>
#include <vector>
#include <chrono>

#include <math.h>
#include <queue>
#include <unordered_set>
#include <map>
#include <thread>
#include <atomic>
#include <mutex>

using namespace cv;
using namespace std;

const auto CAPACITY = 5; // allow max latency of 5 frames
rs2::frame_queue color_data(CAPACITY);
rs2::frame_queue depth_data(CAPACITY);

rs2::pipeline pipe;
rs2::config cfg;

rs2::pointcloud original_pc;
rs2::pointcloud filtered_pc;
    
rs2::colorizer color_map;

std::atomic_bool stopped(false);

void Benchmarking();
void CameraDriver();

void driver(){
    cfg.enable_stream(RS2_STREAM_COLOR, 640, 480, RS2_FORMAT_BGR8, 30);
    cfg.enable_stream(RS2_STREAM_DEPTH, 640, 480, RS2_FORMAT_Z16, 30);
    pipe.start(cfg);

    //Camera Driver Thread
    std::thread camera_driver([&]() { //Gets frames pushes to queue
        CameraDriver();
    });
    camera_driver.detach();
}

std::pair<cv::Mat,cv::Mat> getImage() {
    rs2::frame color_frame;
    rs2::frame depth_frame;

    Mat color;
    Mat depth;
    while (1){
        if (color_data.poll_for_frame(&color_frame)){
            cout << "got frame" << endl;
            color = Mat(Size(640, 480), CV_8UC3, (void*)color_frame.get_data(), Mat::AUTO_STEP);
        }
    }

    while (1){
        if (depth_data.poll_for_frame(&depth_frame)){
            cout << "got frame" << endl;
            color = Mat(Size(640, 480), CV_8UC3, (void*)color_frame.get_data(), Mat::AUTO_STEP);
        }
    }

    return std::make_pair(color, depth);
}