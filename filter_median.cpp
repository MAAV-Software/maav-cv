#include <iostream>
#include <string>
#include <vector>

#include <opencv.hpp>


using namespace cv;
using namespace std;


void filter_median(cv::Mat color, cv::Mat depth) {
    
    for(int i = 0; i < 100; i++){
    
        Mat medianImage;
        medianBlur( color, medianImage, 15 );
        imwrite("./benchmark_filtering_median.jpg", medianImage);
        
    }
}
