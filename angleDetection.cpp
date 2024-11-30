#include "angleDetection.h"
#include <cmath>
#include <iostream>

FindAngle::FindAngle(const std::string& imagePath){
    redImage = extractRed(imagePath);
    double slope = findSlope(redImage);
    angle = calculateAngle(slope);
}

cv::Mat FindAngle::extractRed(std::string path){
     // Load the image
    cv::Mat img = cv::imread(path);
    if (img.empty()) {
        std::cerr << "Could not open or find the image!" << std::endl;
        cv::Mat empty = cv::Mat::zeros(img.size(),CV_8UC1);
        redImage = empty;
        return redImage;
    }

    // Mask to remove white and near-white areas
    cv::Mat nonWhiteMask;
    cv::inRange(img, cv::Scalar(0, 0, 0), cv::Scalar(104, 255, 255), nonWhiteMask);

    // Extract the red channel and apply the non-white mask
    cv::Mat redChannel;
    cv::extractChannel(img, redChannel, 2); // Extract only the red channel
    redChannel = redChannel & nonWhiteMask; // Apply the mask to keep only non-white areas

    // Threshold to isolate red regions
    cv::Mat redThreshold;
    cv::threshold(redChannel, redThreshold, 150, 255, cv::THRESH_BINARY);

    // Create a red-only image by combining with zeroed green and blue channels
    std::vector<cv::Mat> redOnlyChannels = {
        cv::Mat::zeros(img.size(), CV_8UC1),  // Blue channel set to 0
        cv::Mat::zeros(img.size(), CV_8UC1),  // Green channel set to 0
        redThreshold                          // Red channel with threshold applied
    };

    // Merge channels to form the final red-only image
    cv::Mat redOnly;
    cv::merge(redOnlyChannels, redOnly);

    // Save the red-only image
    if (!cv::imwrite("red_channel.jpg", redOnly)) {
        std::cerr << "Could not save the red channel image!" << std::endl;
        cv::Mat empty = cv::Mat::zeros(img.size(),CV_8UC1);
        redImage = empty;
        return redImage;
    }
    std::cout << "Red channel image saved as red_channel.jpg" << std::endl;
    redImage = redOnly;
    return redImage;
}

double FindAngle::findSlope(cv::Mat img){
    cv::Mat gray;
    cv::cvtColor(img, gray, cv::COLOR_BGR2GRAY);

    // Collect the coordinates of all non-zero (active) red pixels
    std::vector<cv::Point> points;
    for (int y = 0; y < gray.rows; y++) {
        for (int x = 0; x < gray.cols; x++) {
            if (gray.at<uchar>(y, x) > 0) {  // Only non-zero pixels
                points.push_back(cv::Point(x, y));
            }
        }
    }

    // Check if there are enough points for a line fit
    if (points.size() < 2) {
        std::cerr << "Not enough red points to fit a line." << std::endl;
        return -1;
    }

    // Calculate the line of best fit using least squares
    double sumX = 0, sumY = 0, sumXY = 0, sumX2 = 0;
    int n = points.size();
    for (const auto& point : points) {
        double x = point.x;
        double y = point.y;
        sumX += x;
        sumY += y;
        sumXY += x * y;
        sumX2 += x * x;
    }

    // Calculate slope (m) and intercept (b) of the line y = mx + b
    //Fix for completely vertical or horizontal position
    double slope = (n * sumXY - sumX * sumY) / (n * sumX2 - sumX * sumX);
    double intercept = (sumY - slope * sumX) / n;
    return slope * -1;
}

double FindAngle::calculateAngle(double slope){
    return atan(slope)*180/ M_PI ;
}

int FindAngle::getAngle(){
    return angle;
}
