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
// Note: if there is an include error, the third party libs aren't built.
// Run ./build-third-party.sh from the cpp directory to build the libs.
// #include "opencv2/opencv.hpp"  // TODO Do we need the whole header?
#include "opencv2/highgui.hpp"
#include "opencv2/imgproc.hpp"
#include "librealsense2/rs.hpp"

using std::cout;
using std::end;
using cv::Mat;

/**
 * Run the camera driver. Intended to be run in a separate thread.
 *
 * Will loop indefinitely, pulling images from the camera and adding to queue
 */
extern void driver();

/**
 * Fetch the next RBG and Depth frames from the queue.
 */
extern std::pair<Mat,Mat> getImage();

// Max latency is 5 frames in the queue
const auto CAPACITY = 5;
