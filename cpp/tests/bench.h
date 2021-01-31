#include <iostream>
#include <string>
#include <vector>

#include <opencv.hpp>

#include "feature_harris.cpp"
#include "feature_orb.cpp"
#include "feature_sift.cpp"
#include "filter_mean.cpp"
#include "filter_median.cpp"
#include "fitting_hough.cpp"

void feature_harris(cv::Mat color, cv::Mat depth);
void feature_orb(cv::Mat color, cv::Mat depth);
void feature_sift(cv::Mat color, cv::Mat depth);
void filter_mean(cv::Mat color, cv::Mat depth);
void filter_median(cv::Mat color, cv::Mat depth);
void fitting_hough(cv::Mat color, cv::Mat depth);
