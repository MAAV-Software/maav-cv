/*
 * rs2_cap.cpp
 * Cheng Jiang
 * chengjia@umich.edu
 * May 2018
 *
 * MAAV Camera Calibration
 * Captures 10 frames with librealsense2. Saves images in the build dir.
 * RGB image saved as png, depth saved as csv. Frame by frame preview is shown.
 *
 * This document is supplied as is without expressed or implied warranties
 * of any kind.
 */


#include <librealsense2/rs.hpp>
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
//#include <opencv4/opencv2>
//#include "opencv2/imgproc/imgproc.hpp"
#include <iostream>
#include <fstream>
#include <chrono>

using namespace rs2;
using namespace cv;

float get_depth_scale(rs2::device dev)
{
    // Go over the device's sensors
    for (rs2::sensor& sensor : dev.query_sensors())
    {
        // Check if the sensor if a depth sensor
        if (rs2::depth_sensor dpt = sensor.as<rs2::depth_sensor>())
        {
            return dpt.get_depth_scale();
        }
    }
    throw std::runtime_error("Device does not have a depth sensor");
}


rs2_stream find_stream_to_align(const std::vector<rs2::stream_profile>& streams)
{
    // Given a vector of streams, we try to find a depth stream and
    // another stream to align depth with.
    // We prioritize color streams to make the view look better.
    // If color is not available, we take another stream that (other than depth)
    rs2_stream align_to = RS2_STREAM_ANY;
    bool depth_stream_found = false;
    bool color_stream_found = false;
    for (rs2::stream_profile sp : streams)
    {
        rs2_stream profile_stream = sp.stream_type();
        if (profile_stream != RS2_STREAM_DEPTH)
        {
            if (!color_stream_found)         //Prefer color
                align_to = profile_stream;

            if (profile_stream == RS2_STREAM_COLOR)
            {
                color_stream_found = true;
            }
        }
        else
        {
            depth_stream_found = true;
        }
    }

    if(!depth_stream_found)
        throw std::runtime_error("No Depth stream available");

    if (align_to == RS2_STREAM_ANY)
        throw std::runtime_error("No stream found to align with Depth");

    return align_to;
}


bool profile_changed(const std::vector<rs2::stream_profile>& current,
                    const std::vector<rs2::stream_profile>& prev)
{
    for (auto&& sp : prev)
    {
        //If previous profile is in current (maybe just added another)
        auto itr = std::find_if(std::begin(current), std::end(current),
            [&sp](const rs2::stream_profile& current_sp) {
                return sp.unique_id() == current_sp.unique_id();
        });
        if (itr == std::end(current)) //If it previous stream wasn't found in current
        {
            return true;
        }
    }
    return false;
}



int main(int argc, char * argv[])
{
    // Declare RealSense pipeline, encapsulating the actual device and sensors
    rs2::pipeline pipe;
    rs2::config cfg;
    cfg.enable_stream(RS2_STREAM_COLOR, 640, 480, RS2_FORMAT_BGR8, 30);
    cfg.enable_stream(RS2_STREAM_DEPTH, 640, 480, RS2_FORMAT_Z16, 30);
    rs2::pipeline_profile profile = pipe.start(cfg);
    float depth_scale = get_depth_scale(profile.get_device());
    rs2_stream align_to = find_stream_to_align(profile.get_streams());
    rs2::align align(align_to);

    auto depth_in = profile.get_stream(RS2_STREAM_DEPTH).
        as<rs2::video_stream_profile>().get_intrinsics();

    auto color_in = profile.get_stream(RS2_STREAM_COLOR).
        as<rs2::video_stream_profile>().get_intrinsics();

    std::cout << "{\n"
              << "    \"model\": \"d415\",\n"
              << "    \"depth\": {\n"
              << "        \"fx\": " << depth_in.fx << ",\n"
              << "        \"fy\": " << depth_in.fy << ",\n"
              << "        \"ppx\": " << depth_in.ppx << ",\n"
              << "        \"ppy\": " << depth_in.ppy << "\n"
              << "    },\n"
              << "    \"color\": {\n"
              << "        \"fx\": " << color_in.fx << ",\n"
              << "        \"fy\": " << color_in.fy << ",\n"
              << "        \"ppx\": " << color_in.ppx << ",\n"
              << "        \"ppy\": " << color_in.ppy << "\n"
              << "    }\n"
              << "}" << std::endl;

    // Capture 30 frames to give autoexposure, etc. a chance to settle
    for (auto i = 0; i < 30; ++i) pipe.wait_for_frames();

    for (size_t i {0}; i < 10; ++i)
    {
        unsigned long start =
            std::chrono::system_clock::now().time_since_epoch() /
            std::chrono::milliseconds(1);

        rs2::frameset frames = pipe.wait_for_frames();
        if (profile_changed(pipe.get_active_profile().get_streams(),
                            profile.get_streams()))
        {
            // If the profile was changed, update the align object,
            // and also get the new device's depth scale
            profile = pipe.get_active_profile();
            align_to = find_stream_to_align(profile.get_streams());
            align = rs2::align(align_to);
            depth_scale = get_depth_scale(profile.get_device());
        }

        auto processed = align.process(frames);

        // Trying to get both color and aligned depth frames
        rs2::video_frame other_frame = processed.first_or_default(align_to);
        rs2::depth_frame aligned_depth_frame = processed.get_depth_frame();

        //If one of them is unavailable, continue iteration
        if (!aligned_depth_frame || !other_frame)
        {
            continue;
        }


        cv::Mat color(cv::Size(640, 480), CV_8UC3,
            (void*)other_frame.get_data(), cv::Mat::AUTO_STEP);
        cv::Mat depth(cv::Size(640, 480), CV_16SC1,
            (void*)aligned_depth_frame.get_data(), cv::Mat::AUTO_STEP);
        cv::cvtColor(color, color, COLOR_BGR2RGB);

        unsigned long end =
            std::chrono::system_clock::now().time_since_epoch() /
            std::chrono::milliseconds(1);
        auto diff = end - start;
        std::cout<<"Elapsed time is :  " << diff << " ms " << std::endl;

        std::ofstream fout;
        std::string out_name = "depth" + std::to_string(i) + ".txt";
        std::string color_name = "rgb" + std::to_string(i) + ".png";

        std::vector<int> compression_params;
        compression_params.push_back(cv::IMWRITE_PNG_COMPRESSION);
        compression_params.push_back(1);

        fout.open(out_name.c_str());
        fout << cv::format(depth, cv::Formatter::FMT_CSV) << std::endl;
        cv::imwrite(color_name, color, compression_params);

        //cv::Mat mask = depth >= 5000;
        //depth.setTo(5000, mask);

        double min,  max;
        cv::minMaxIdx(depth, &min, &max);
        cv::Mat depthvis;
        depth.convertTo(depthvis, CV_8UC1, 255 / (max-min), -min);
        applyColorMap(depthvis, depthvis, cv::COLORMAP_RAINBOW);

        cv::imshow("col", color);
        cv::imshow("D", depthvis);
        cv::waitKey();

        fout.close();
    }

    return EXIT_SUCCESS;
}
