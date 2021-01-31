#include <iostream>
#include <string>
#include <vector>

#include <opencv.hpp>


using namespace cv;
using namespace std;


void filter_mean(cv::Mat color, cv::Mat depth) {
    
    for(int i = 0; i < 100; i++){
        Mat avgImage;
        // Perform mean filtering on image using boxfilter
        boxFilter(color, avgImage, -1, cv::Size(3,3));
        imwrite("./benchmark_filtering_mean.jpg", avgImage);
    }
}
