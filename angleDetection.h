#ifndef ANGLEDETECTION_H
#define ANGLEDETECTION_H

#include <string>
#include <opencv2/opencv.hpp>

class FindAngle{
public:
    FindAngle(const std::string& imagePath);
    int getAngle();

private:
    double angle;
    cv::Mat redImage;
    cv::Mat extractRed(std::string path);
    double findSlope(cv::Mat img);
    double calculateAngle(double slope);
};

#endif