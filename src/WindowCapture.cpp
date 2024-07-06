#include <cstdio>
#include <iostream>

#include "WindowCapture.hpp"
#include "opencv2/imgproc.hpp"
#include "opencv2/imgproc/types_c.h"

#define BORDER_PIXELS 8
#define TITLEBAR_PIXELS 30

static BOOL CALLBACK enumWindowCallback(HWND hwnd, LPARAM lparam) {
  uint32_t length = GetWindowTextLength(hwnd);
  char* buffer = new char[length + 1];
  GetWindowText(hwnd, buffer, length + 1);
  std::string windowTitle(buffer);
  delete[] buffer;

  if (IsWindowVisible(hwnd) && length)
    std::cout << hwnd << ": " << windowTitle << "\n";

  return TRUE;
}

WindowCapture::WindowCapture() {
  this->hwnd = nullptr;
}

WindowCapture::WindowCapture(unsigned long long hwnd) {
  this->hwnd = (HWND)hwnd;
}

void WindowCapture::printVisibleWindows() {
  EnumWindows(enumWindowCallback, NULL);
}

// Taking a screenshot from the whole screen then cropping the sub image from it
cv::Mat WindowCapture::getScreenshot() const {
  cv::Mat src;
  HWND screenHWND = GetDesktopWindow();

  // get handles to a device context (DC)
  HDC hwindowDC = GetDC(screenHWND);
  HDC hwindowCompatibleDC = CreateCompatibleDC(hwindowDC);
  SetStretchBltMode(hwindowCompatibleDC, COLORONCOLOR);

  RECT appRect;
  GetWindowRect(hwnd, &appRect);
  int screenX = appRect.left;
  int screenY = appRect.top;
  int width   = appRect.right - appRect.left;
  int height  = appRect.bottom - appRect.top;

  // create mat object
  src.create(height, width, CV_8UC4);

  // create a bitmap
  HBITMAP hbwindow = CreateCompatibleBitmap(hwindowDC, width, height);
  BITMAPINFOHEADER bi;
  bi.biSize = sizeof(BITMAPINFOHEADER);
  bi.biWidth = width;
  bi.biHeight = -height;  //this is the line that makes it draw upside down or not
  bi.biPlanes = 1;
  bi.biBitCount = 32;
  bi.biCompression = BI_RGB;
  bi.biSizeImage = 0;
  bi.biXPelsPerMeter = 0;
  bi.biYPelsPerMeter = 0;
  bi.biClrUsed = 0;
  bi.biClrImportant = 0;

  // use the previously created device context with the bitmap
  SelectObject(hwindowCompatibleDC, hbwindow);

  // copy from the window device context to the bitmap device context
  StretchBlt(hwindowCompatibleDC, 0, 0, width, height, hwindowDC, screenX, screenY, width, height, SRCCOPY);  //change SRCCOPY to NOTSRCCOPY for wacky colors !
  GetDIBits(hwindowCompatibleDC, hbwindow, 0, height, src.data, (BITMAPINFO*)&bi, DIB_RGB_COLORS);            //copy from hwindowCompatibleDC to hbwindow

  // avoid memory leak
  DeleteObject(hbwindow);
  DeleteDC(hwindowCompatibleDC);
  ReleaseDC(screenHWND, hwindowDC);

  cv::cvtColor(src, src, CV_BGRA2BGR);
  src.convertTo(src, CV_8UC3);

  return src;
}

vec2i WindowCapture::getScreenPosition(vec2i pos) const {
  RECT rect;
  GetClientRect(hwnd, &rect);

  return pos + vec2i{rect.left + BORDER_PIXELS, rect.top + TITLEBAR_PIXELS};
};

