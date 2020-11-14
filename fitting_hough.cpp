#include <iostream>
#include <string>
#include <vector>

#include <opencv.hpp>


using namespace cv;
using namespace std;


void fitting_hough(cv::Mat color, cv::Mat depth){

    for(int i = 0; i < 33; i++){
        
        Mat dst, cdst, cdstP, src;
        
        cvtColor(color, src, COLOR_BGR2GRAY);

        // Edge detection
        Canny(src, dst, 50, 200, 3);
        // Copy edges to the images that will display the results in BGR
        cvtColor(dst, cdst, COLOR_GRAY2BGR);
        cdstP = cdst.clone();
        // Standard Hough Line Transform
        vector<Vec2f> lines; // will hold the results of the detection
        HoughLines(dst, lines, 1, CV_PI/180, 150, 0, 0 ); // runs the actual detection
        // Draw the lines
        for( size_t i = 0; i < lines.size(); i++ )
        {
            float rho = lines[i][0], theta = lines[i][1];
            Point pt1, pt2;
            double a = cos(theta), b = sin(theta);
            double x0 = a*rho, y0 = b*rho;
            pt1.x = cvRound(x0 + 1000*(-b));
            pt1.y = cvRound(y0 + 1000*(a));
            pt2.x = cvRound(x0 - 1000*(-b));
            pt2.y = cvRound(y0 - 1000*(a));
            line( cdst, pt1, pt2, Scalar(0,0,255), 3, LINE_AA);
        }

        // Show results
        imwrite("./benchmark_fitting_hough.jpg", cdst);

    }
}
