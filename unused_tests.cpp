#include <iostream>
#include <string>
#include <vector>

#include <opencv.hpp>


using namespace cv;
using namespace std;


void benchmark_filtering(cv::Mat color, cv::Mat depth) {
    
    // Gaussian filtering
    Mat blurredImage;
    GaussianBlur( color, blurredImage, Size( 9, 9 ), 1.0);
    imwrite("./benchmark_filtering_gauss.jpg", blurredImage);
    
    
    Mat kernel;   // Use the default structuring element (kernel) for erode and dilate
    
    Mat minImage;
    // Perform min filtering on image using erode
    erode(color, minImage, kernel);
    imwrite("./benchmark_filtering_min.jpg", minImage);

    Mat maxImage;
    // Perform max filtering on image using dilate
    dilate(color, maxImage, kernel);
    imwrite("./benchmark_filtering_max.jpg", maxImage);
    
}

void benchmark_fitting(cv::Mat color, cv::Mat depth) {
    
    // Homography & RANSAC
    
    // Four corners in source image
    vector<Point2f> pts_src;
    pts_src.push_back(Point2f(141, 131));
    pts_src.push_back(Point2f(480, 159));
    pts_src.push_back(Point2f(493, 630));
    pts_src.push_back(Point2f(64, 601));

    // Read destination image.
    Mat im_dst = color;
    // Four corners in destination image.
    vector<Point2f> pts_dst;
    pts_dst.push_back(Point2f(318, 256));
    pts_dst.push_back(Point2f(534, 372));
    pts_dst.push_back(Point2f(316, 670));
    pts_dst.push_back(Point2f(73, 473));
    
    // Calculate Homography
    Mat h = findHomography(pts_src, pts_dst);
    Mat h2 = findHomography(pts_src, pts_dst, RANSAC, 10);

    // Output image
    Mat im_out;
    Mat im_out2;
    // Warp source image to destination based on homography
    warpPerspective(color, im_out, h, im_dst.size());
    warpPerspective(color, im_out2, h2, im_dst.size());

    imwrite("./benchmark_fitting_homography.jpg", im_out);
    imwrite("./benchmark_fitting_ransac.jpg", im_out2);
    
    
    // Canny
    Mat imgCanny;
    Mat imgBlurred;
    Mat imgGrayscale;
    
    cvtColor(color, imgGrayscale, COLOR_BGR2GRAY);
    
    GaussianBlur(imgGrayscale, imgBlurred, Size(5, 5), 1.5);
    
    cv::Canny(imgBlurred, imgCanny, 100, 200);
    
    imwrite("./benchmark_fitting_canny.jpg", imgCanny);
}
