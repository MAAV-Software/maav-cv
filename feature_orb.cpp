#include <iostream>
#include <string>
#include <vector>

#include <opencv.hpp>


using namespace cv;
using namespace std;

void feature_orb(cv::Mat color, cv::Mat depth) {

    // Orb
    vector<KeyPoint> kpts1;
    vector<KeyPoint> kpts2;
    vector<DMatch> matches;
    
    const double kDistanceCoef = 4.0;
    const int kMaxMatchingSize = 50;
    
    Mat desc1;
    Mat desc2;

    for(int i = 0; i < 100; i++){
        
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
}
