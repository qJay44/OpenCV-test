#pragma once

#include <string>
#include <list>

#include "opencv2/core/mat.hpp"
#include "opencv2/imgproc.hpp"

#define DRAW_TYPE_RECTANGLE 0x1
#define DRAW_TYPE_MARKER 0x2

class Vision {
  public:
    Vision(std::string imgSubPath, uint32_t method = cv::TM_CCOEFF_NORMED);

    void find(const cv::Mat& img, double threshold, double closeness);
    void draw(cv::Mat& img, uint8_t drawType, cv::Scalar color = cv::Scalar(0, 255, 0), int thickness = 1) const;
    void clear(); // clear locations
    void setMarkerType(uint8_t type);

  private:
    cv::Mat imgSub;
    uint32_t w;
    uint32_t h;
    uint32_t method;
    uint8_t markerType = cv::MARKER_CROSS;
    std::list<cv::Point> locations;
};

