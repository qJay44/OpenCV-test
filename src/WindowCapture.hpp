#pragma once

#include <Windows.h>

#include "opencv2/core/mat.hpp"

class WindowCapture {
  public:
    WindowCapture();
    WindowCapture(unsigned long long hwnd);

    static void printVisibleWindows();

    cv::Mat getScreenshot() const;
    vec2i getScreenPosition(vec2i pos) const;

  private:
    HWND hwnd;
    vec2i offset;
};

