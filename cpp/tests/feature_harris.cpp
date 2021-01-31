#include <iostream>
#include <string>
#include <vector>

#include <opencv.hpp>


using namespace cv;
using namespace std;

void feature_harris(cv::Mat color, cv::Mat depth) {

    for(int i = 0; i < 100; i++){
        // Harris
        Mat src_gray;
        const Mat src = color;
        cvtColor( src, src_gray, COLOR_BGR2GRAY );
        int thresh = 200;
        int blockSize = 2;
        int apertureSize = 3;
        double k = 0.04;
    
        Mat dst = Mat::zeros( src.size(), CV_32FC1 );
        cornerHarris( src_gray, dst, blockSize, apertureSize, k );
        Mat dst_norm, dst_norm_scaled;
        normalize( dst, dst_norm, 0, 255, NORM_MINMAX, CV_32FC1, Mat() );
        convertScaleAbs( dst_norm, dst_norm_scaled );
        for( int i = 0; i < dst_norm.rows ; i++ )
        {
            for( int j = 0; j < dst_norm.cols; j++ )
            {
                if( (int) dst_norm.at<float>(i,j) > thresh )
                {
                    circle( dst_norm_scaled, Point(j,i), 5,  Scalar(0), 2, 8, 0 );
                }
            }
        }
        imwrite("./benchmark_feature_harris.jpg", dst_norm);
    }
}

