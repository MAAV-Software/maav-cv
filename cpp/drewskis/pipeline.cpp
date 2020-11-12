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
#include "Driver.h"

using namespace cv;
using namespace std;

void update_data(rs2::frame_queue& data, rs2::frame& depth, rs2::colorizer& color_map);
void show_image(Mat color, Mat depth);
void CameraDriver();
void Benchmarking();

int main(int argc, char** argv){
    driver(); //Starts the camera driver
    
    //Main Thread
    Benchmarking();
}

void CameraDriver() {
    while (!stopped){
        rs2::frameset data = pipe.wait_for_frames();
        rs2::frame color_frame = data.get_color_frame();
        rs2::frame depth_frame = data.get_depth_frame();
        

        color_data.enqueue(color_frame);
        depth_data.enqueue(depth_frame);     
    }
}

void Benchmarking() {
    std::pair<Mat, Mat> frame_pair;
    while (!stopped){ //pulls from queue and does processing
        rs2::frame color_frame;
        rs2::frame depth_frame;


        frame_pair = getImage();
        Mat color = std::get<0>(frame_pair);
        Mat depth = std::get<1>(frame_pair);
        show_image(color, depth);
        

        /*
        if (color_data.poll_for_frame(&color_frame)){
            cout << "got frame" << endl;
            Mat color(Size(640, 480), CV_8UC3, (void*)color_frame.get_data(), Mat::AUTO_STEP);
            namedWindow("Display Image", WINDOW_AUTOSIZE);
            imshow("Display Image", color);
            waitKey(1);
        }
        */
    }
}


void show_image(Mat color, Mat depth){
    //namedWindow("Display Image", WINDOW_AUTOSIZE);
    imshow("Display Image", color);
    //imshow("Display Image", depth);
    waitKey(1);
    
    destroyAllWindows();
}

/*
void update_data(rs2::frame_queue& data, rs2::frame& depth, rs2::colorizer& color_map){
    rs2::frame f;
    if (data.poll_for_frame(&f)){
        depth = f;
    }
}
*/


//Check for exit
        /*
        if((char)cv::waitKey(1) == 27){
            cout << "esc pressed" << endl;
            stopped = true;
            return 0;
        }
        */