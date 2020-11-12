#include <librealsense2/rs.hpp>
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include <thread>
#include <mutex>
#include <queue>
#include <deque>
#include <condition_variable>
#include <iostream>
#include <fstream>
#include <chrono>
#include <unistd.h>
using namespace std;
using namespace rs2;
using namespace cv;

std::deque<cv::Mat> colorImages;
std::deque<cv::Mat> depthImages;
std::mutex mutexColor;
std::mutex mutexDepth;
std::condition_variable emptyColor;
std::condition_variable emptyDepth;
//want to define these as extern variables in benchmark file


void cameraDriver()
{
  // Declare RealSense pipeline, encapsulating the actual device and sensors
  rs2::pipeline pipe;
  rs2::config cfg;
  cfg.enable_stream(RS2_STREAM_COLOR, 640, 480, RS2_FORMAT_BGR8, 30);
  cfg.enable_stream(RS2_STREAM_DEPTH, 640, 480, RS2_FORMAT_Z16, 30);
  rs2::pipeline_profile profile = pipe.start(cfg);


  // Capture 30 frames to give autoexposure, etc. a chance to settle
  for (auto i = 0; i < 30; ++i) pipe.wait_for_frames();
  while (1)
  {

    rs2::frameset frames = pipe.wait_for_frames();

    // Trying to get both color and aligned depth frames
    auto color_frame = frames.get_color_frame();
    auto depth_frame = frames.get_depth_frame();


    cv::Mat color(cv::Size(640, 480), CV_8UC3,
    (void*)color_frame.get_data(), cv::Mat::AUTO_STEP);
    cv::Mat depth(cv::Size(640, 480), CV_16SC1,
    (void*)depth_frame.get_data(), cv::Mat::AUTO_STEP);
    //std::unique_lock<std::mutex> lock1(mutexColor,  std::defer_lock);
    //std::unique_lock<std::mutex> lock2(mutexDepth,  std::defer_lock);

    mutexColor.lock();
    colorImages.push_back(color.clone());
    mutexColor.unlock();
    emptyColor.notify_all();


    mutexDepth.lock();
    depthImages.push_back(depth.clone());
    mutexDepth.unlock();
    emptyDepth.notify_all();

  }

}


//returns pair containing color and depth images
//color can be accessed with .first
//depth can be accessed with .second
std::pair<cv::Mat, cv::Mat> getImage() {
  std::unique_lock<std::mutex> lock1(mutexColor,  std::defer_lock);
  std::unique_lock<std::mutex> lock2(mutexDepth,  std::defer_lock);
  lock1.lock();
  while(colorImages.empty()) {
    emptyColor.wait(lock1);
  }
  auto color = colorImages.front();
  colorImages.pop_front();

  lock1.unlock();
  lock2.lock();
  while(depthImages.empty()) {
    emptyDepth.wait(lock2);
  }
  auto depth = depthImages.front();
  depthImages.pop_front();
  lock2.unlock();
  return std::make_pair(color, depth);
}
