#include <opencv2/core.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>

#include "Vision.hpp"
#include "WindowCapture.hpp"

int main() {
  WindowCapture::printVisibleWindows();
  WindowCapture win(0x0000000002520602);
  Vision vision("../../src/res/test2.jpg");
  cv::Mat img, result;

  while (true) {
    img = win.getScreenshot();
    result = img.clone();

    vision.clear();
    vision.find(img, 0.8, 1.0);
    vision.draw(result, DRAW_TYPE_RECTANGLE, cv::Scalar(255, 0, 255), 3);

    cv::imshow("Result", result);

    if (cv::waitKey(1) == 'q' || !cv::getWindowProperty("Result", cv::WND_PROP_VISIBLE))
      break;
  }

  return EXIT_SUCCESS;
}

