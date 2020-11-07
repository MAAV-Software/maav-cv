#include <opencv2/opencv.hpp>
#include <opencv2/highgui.hpp>
#include <librealsense2/rs.hpp>
#include <iostream>
#include <vector>
#include <chrono>

#include <imgui.h>
#include "./imgui/imgui/backends/imgui_impl_glfw.h"



#include <math.h>
#include <queue>
#include <unordered_set>
#include <map>
#include <thread>
#include <atomic>
#include <mutex>

using namespace cv;
using namespace std;


int main(int argc, char** argv){

    std::mutex queue_lock;
    std::condition_variable queue_empty;

    rs2::pipeline pipe;
    rs2::config cfg;
    cfg.enable_stream(RS2_STREAM_COLOR, 640, 480, RS2_FORMAT_BGR8, 30);
    pipe.start(cfg);

    const auto CAPACITY = 5; // allow max latency of 5 frames
    rs2::frame_queue original_data;
    rs2::frame_queue filtered_data;

    rs2::pointcloud original_pc;
    rs2::pointcloud filtered_pc;
    
    rs2::colorizer color_map;

    std::atomic_bool stopped(false);

    std::thread processing_boi([&]() { 
        while (!stopped){
            rs2::frameset data = pipe.wait_for_frames();
            rs2::frame color_frame = data.get_color_frame();
            
            if (!color_frame){
                cout << "Not a thing" << endl;
                return;
            }

            rs2::frame filtered = color_frame;

            //CAN APPLY SOME FILTERS HERE

            filtered_data.enqueue(filtered);
            original_data.enqueue(color_frame);     
        }
    });

    processing_boi.detach();

    while (true){
        rs2::frame color_frame;
        rs2::frame color_filtered;

        auto last_time = std::chrono::high_resolution_clock::now(); //Last frame arrived time
        
        if (original_data.poll_for_frame(&color_frame)){
            cout << "got frame" << endl;
            Mat color(Size(640, 480), CV_8UC3, (void*)color_frame.get_data(), Mat::AUTO_STEP);
            namedWindow("Display Image", WINDOW_AUTOSIZE);
            imshow("Display Image", color);
            waitKey(1);
        }
        
        auto this_time = std::chrono::high_resolution_clock::now();

        //auto duration = std::chrono::duration_cast<std::chrono::microseconds>(this_time - last_time);
        //cout << duration.count() << endl;

        
    }
}

void update_data(rs2::frame_queue& data, rs2::frame& depth, rs2::colorizer& color_map){
    rs2::frame f;
    if (data.poll_for_frame(&f)){
        depth = f;
    }
}
