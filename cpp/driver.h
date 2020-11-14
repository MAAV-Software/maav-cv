#include <iostream>
#include <chrono>
#include <math.h>

// Multithreading stuff
#include <atomic>
#include <condition_variable>
#include <mutex>
#include <thread>

// Data structs
#include <deque>
#include <map>
#include <queue>
#include <vector>

// Third party
// From drewskis
#include <opencv2/opencv.hpp>
#include <opencv2/highgui.hpp>
#include <librealsense2/rs.hpp>
// From chereddy
#include <librealsense2/rs.hpp>
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"

using std::cout;
using std::end;
using cv::Mat;

/**
 * Run the camera driver. Intended to be run in a separate thread.
 *
 * Will loop indefinitely, pulling images from the camera and adding to queue
 */
void driver();

/**
 * Fetch the next RBG and Depth frames from the queue.
 */
std::pair<Mat,Mat> getImage();

// Max latency is 15 frames in the queue
const auto CAPACITY = 15;
