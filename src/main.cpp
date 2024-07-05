#include <opencv2/core.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>

#include "Vision.hpp"
#include "WindowCapture.hpp"

int main() {

  WindowCapture::printVisibleWindows();
  WindowCapture win(0x00000000000205C2);
  Vision vision("../../src/res/albion_cabbage.jpg");

  const cv::Mat img = win.getScreenshot();
  cv::Mat result = img.clone();

  vision.find(img, 0.4, 1.0);
  vision.draw(result, DRAW_TYPE_RECTANGLE);

  cv::imshow("Result", result);
  cv::waitKey();
  /* while (cv::waitKey(1) != 'q' || cv::getWindowProperty("Result", cv::WND_PROP_VISIBLE)) { */
  /* } */

  return EXIT_SUCCESS;
}

