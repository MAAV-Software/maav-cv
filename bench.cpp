#include <iostream>
#include <string>
#include <vector>

#include <opencv.hpp>
#include "bench.h"

using namespace cv;
using namespace std;

int main(){
    
    Mat color, depth;
    
    color = imread("rgb94.png");

    filter_mean(color, depth);
    filter_median(color, depth);
    feature_harris(color, depth);
    feature_orb(color, depth);
    feature_sift(color, depth);
    fitting_hough(color, depth);

    return 0;
}

