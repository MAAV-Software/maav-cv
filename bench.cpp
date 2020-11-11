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
    
    Mat avgImage;
    // Perform mean filtering on image using boxfilter
    boxFilter(color, avgImage, -1, cv::Size(3,3));
    imwrite("./benchmark_filtering_mean.jpg", avgImage);
    
    Mat kernel;   // Use the default structuring element (kernel) for erode and dilate
    
    Mat medianImage;
    medianBlur( color, medianImage, 15 );
    imwrite("./benchmark_filtering_median.jpg", medianImage);
    
    Mat minImage;
    // Perform min filtering on image using erode
    erode(color, minImage, kernel);
    imwrite("./benchmark_filtering_min.jpg", minImage);

    Mat maxImage;
    // Perform max filtering on image using dilate
    dilate(color, maxImage, kernel);
    imwrite("./benchmark_filtering_max.jpg", maxImage);
    
}

void benchmark_feature(cv::Mat color, cv::Mat depth) {

    // Sift
    Ptr<SiftFeatureDetector> detector = SiftFeatureDetector::create();
    vector<KeyPoint> keypoints;
    detector->detect(color, keypoints);

    Mat output;
    drawKeypoints(color, keypoints, output);
    imwrite("./benchmark_feature_sift.jpg", output);

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
    
    imwrite("./benchmark_feature_harris.jpg", output);
    
    // Orb
    vector<KeyPoint> kpts1;
    vector<KeyPoint> kpts2;
    vector<DMatch> matches;
    
    const double kDistanceCoef = 4.0;
    const int kMaxMatchingSize = 50;
    
    Mat desc1;
    Mat desc2;
    //-- 初始化
    Ptr<ORB> orb = ORB::create();
    orb->detectAndCompute(color, Mat(), kpts1, desc1);
    orb->detectAndCompute(color, Mat(), kpts2, desc2);

    BFMatcher desc_matcher(cv::NORM_L2, true);
    desc_matcher.match(desc1, desc2, matches, Mat());
    
    std::sort(matches.begin(), matches.end());
    while (matches.front().distance * kDistanceCoef < matches.back().distance) {
        matches.pop_back();
    }
    while (matches.size() > kMaxMatchingSize) {
        matches.pop_back();
    }
    
    vector<char> match_mask(matches.size(), 1);
    
    vector<Point2f> pts1;
    vector<Point2f> pts2;
    for (int i = 0; i < static_cast<int>(matches.size()); ++i) {
        pts1.push_back(kpts1[matches[i].queryIdx].pt);
        pts2.push_back(kpts2[matches[i].trainIdx].pt);
    }
    findHomography(pts1, pts2, cv::RANSAC, 4, match_mask);

    Mat res;
    cv::drawMatches(color, kpts1, color, kpts2, matches, res, Scalar::all(-1),
        Scalar::all(-1), match_mask, DrawMatchesFlags::NOT_DRAW_SINGLE_POINTS);

    imwrite("./benchmark_feature_orb.jpg", res);
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

int main(){
    
    Mat color, depth;
    
    color = imread("mast-aerial.png");
    
    benchmark_filtering(color, depth);
    benchmark_feature(color, depth);
    benchmark_fitting(color, depth);
    return 1;
}

