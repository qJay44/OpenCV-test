#include "Vision.hpp"

#include "opencv2/core.hpp"
#include "opencv2/core/mat.hpp"
#include "opencv2/imgcodecs.hpp"
#include "opencv2/imgproc.hpp"

Vision::Vision(std::string imgSubPath, uint32_t method) {
  imgSub = cv::imread(imgSubPath, cv::IMREAD_COLOR);
  w = imgSub.rows;
  h = imgSub.cols;
  this->method = method;
  printf("imgSub: %d\n", imgSub.type());
}

void Vision::find(const cv::Mat& img, double threshold, double closeness) {
  cv::Mat result, imgGray, imgSubGray;

  cv::cvtColor(img, imgGray, cv::COLOR_BGR2GRAY);
  cv::cvtColor(imgSub, imgSubGray, cv::COLOR_BGR2GRAY);

  matchTemplate(imgGray, imgSubGray, result, method);
  cv::threshold(result, result, threshold, 1.0, cv::THRESH_TOZERO);

  cv::Size2i imgSubSize = imgSubGray.size();
  cv::Size2i imgSubCloseRadius(
    static_cast<int>(imgSubSize.width * 0.5 * closeness),
    static_cast<int>(imgSubSize.height * 0.5 * closeness)
  );
  double mx = 0.;

  while (true) {
    double minVal, maxVal;
    cv::Point minLoc, maxLoc;

    cv::minMaxLoc(result, &minVal, &maxVal, &minLoc, &maxLoc);

    if (maxVal >= threshold) {
      cv::Point2f pos{
        static_cast<float>(maxLoc.x - imgSubCloseRadius.width),
        static_cast<float>(maxLoc.y - imgSubCloseRadius.height),
      };
      cv::Point2f size{
        static_cast<float>(maxLoc.x + imgSubCloseRadius.width),
        static_cast<float>(maxLoc.y + imgSubCloseRadius.height),
      };

      cv::rectangle(result, pos, size, cv::Scalar(0), -1);
      locations.push_back(maxLoc);
    } else break;
  }
}

void Vision::draw(cv::Mat& img, uint8_t drawType, cv::Scalar color, int thickness) const {
  for (const cv::Point& p : locations) {
    if (drawType & DRAW_TYPE_RECTANGLE)
      cv::rectangle(img, p, cv::Point(p.x + w, p.y + h), cv::Scalar(0, 255, 0), 1);
    if (drawType & DRAW_TYPE_MARKER)
      cv::drawMarker(img, p, cv::Scalar(0, 255, 0), markerType);
  }
}

void Vision::clear() {
  locations.clear();
}

void Vision::setMarkerType(uint8_t type) {
  markerType = type;
}

