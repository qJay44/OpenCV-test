#include <opencv2/core.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>

#include "WindowCapture.hpp"

int main() {

  WindowCapture::printVisibleWindows();
  WindowCapture wnd(0x00000000003004BC);
  cv::imshow("Result", wnd.getScreenshot());

  cv::waitKey();
  /* while (cv::waitKey(1) != 'q' || cv::getWindowProperty("Result", cv::WND_PROP_VISIBLE)) { */
  /* } */

  return EXIT_SUCCESS;
}

