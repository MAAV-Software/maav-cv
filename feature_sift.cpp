#include <iostream>
#include <string>
#include <vector>

#include <opencv.hpp>


using namespace cv;
using namespace std;

void feature_sift(cv::Mat color, cv::Mat depth) {
    
    for(int i = 0; i < 100; i++){

        // Sift
        Ptr<SiftFeatureDetector> detector = SiftFeatureDetector::create();
        vector<KeyPoint> keypoints;
        detector->detect(color, keypoints);

        Mat output;
        drawKeypoints(color, keypoints, output);
        imwrite("./benchmark_feature_sift.jpg", output);
        
    }
}
