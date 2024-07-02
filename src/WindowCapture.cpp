#include <iostream>

#include "WindowCapture.hpp"

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

cv::Mat WindowCapture::getScreenshot() const {
  HDC hwindowDC,hwindowCompatibleDC;

  int height,width,srcheight,srcwidth;
  HBITMAP hbwindow;
  cv::Mat src;
  BITMAPINFOHEADER bi;

  hwindowDC = GetDC(hwnd);
  hwindowCompatibleDC = CreateCompatibleDC(hwindowDC);
  SetStretchBltMode(hwindowCompatibleDC,COLORONCOLOR);

  RECT windowsize;    // get the height and width of the screen
  GetClientRect(hwnd, &windowsize);

  float valueOfChangeTheSizeOfTextAppsAndOtherItemsInWindowsDisplaySettings = 1.0f;
  srcheight = (int)((float)windowsize.bottom * valueOfChangeTheSizeOfTextAppsAndOtherItemsInWindowsDisplaySettings);
  srcwidth = (int)((float)windowsize.right * valueOfChangeTheSizeOfTextAppsAndOtherItemsInWindowsDisplaySettings);
  height = windowsize.bottom/1;  //change this to whatever size you want to resize to
  width = windowsize.right/1;

  src.create(height,width,CV_8UC4);

  // create a bitmap
  hbwindow = CreateCompatibleBitmap( hwindowDC, width, height);
  bi.biSize = sizeof(BITMAPINFOHEADER);    //http://msdn.microsoft.com/en-us/library/windows/window/dd183402%28v=vs.85%29.aspx
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
  StretchBlt(hwindowCompatibleDC, 0, 0, width, height, hwindowDC, 0, 0, srcwidth, srcheight, SRCCOPY); //change SRCCOPY to NOTSRCCOPY for wacky colors !
  GetDIBits(hwindowCompatibleDC, hbwindow, 0, height, src.data, (BITMAPINFO *)&bi, DIB_RGB_COLORS);  //copy from hwindowCompatibleDC to hbwindow

  // avoid memory leak
  DeleteObject (hbwindow);
  DeleteDC(hwindowCompatibleDC);
  ReleaseDC(hwnd, hwindowDC);

  return src;
}

vec2i WindowCapture::getScreenPosition(vec2i pos) const {
  RECT rect;
  GetClientRect(hwnd, &rect);

  return pos + vec2i{rect.left + BORDER_PIXELS, rect.top + TITLEBAR_PIXELS};
};

