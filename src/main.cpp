#include <cstdlib>
#include <cstdio>

#include <opencv2/core.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>

cv::Mat& scanImageAndReduce_C(cv::Mat& I, const uchar* const table);
cv::Mat& scanImageAndReduce_Iterator(cv::Mat& I, const uchar* const table);
cv::Mat& scanImageAndReduce_RandomAccess(cv::Mat& I, const uchar* const table);

int main() {
  std::string imgPath = cv::samples::findFile("exampleImage.png");
  cv::Mat img;
  img = imread(imgPath, cv::IMREAD_COLOR);

  if(img.empty()) {
    printf("Could not read the image");
    return EXIT_FAILURE;
  }

  namedWindow("Display window", cv::WINDOW_AUTOSIZE);
  imshow("Display window", img);

  uint divideWith = 10; // Comes from the input
  uchar table[256];
  for (int i = 0; i < 256; i++) {
    table[i] = (uchar)(divideWith * (i / divideWith));
  }

  int k = cv::waitKey(0);
  if (k == 's')
    imwrite("starry_night.png", img);

  return EXIT_SUCCESS;
}

cv::Mat& scanImageAndReduce_C(cv::Mat& I, const uchar* const table) {
  CV_Assert(I.depth() == CV_8U);

  uint channels = I.channels();
  uint nRows = I.rows;
  uint nCols = I.cols * channels;

  if (I.isContinuous()) {
    nCols *= nRows;
    nRows = 1;
  }

  uint i, j;
  uchar* p;
  for (i = 0; i < nRows; i++) {
    p = I.ptr<uchar>(i);
    for (j = 0; j < nCols; j++)
      p[j] = table[p[j]];
  }

  return I;
}

cv::Mat& scanImageAndReduce_Iterator(cv::Mat& I, const uchar* const table) {
  CV_Assert(I.depth() == CV_8U);

  switch(I.channels()) {
    case 1: {
      cv::MatIterator_<uchar> it, end;
      for(it = I.begin<uchar>(), end = I.end<uchar>(); it != end; ++it)
        *it = table[*it];
      break;
    }
    case 3: {
      cv::MatIterator_<cv::Vec3b> it, end;
      for(it = I.begin<cv::Vec3b>(), end = I.end<cv::Vec3b>(); it != end; ++it) {
        (*it)[0] = table[(*it)[0]];
        (*it)[1] = table[(*it)[1]];
        (*it)[2] = table[(*it)[2]];
      }
      break;
    }
  }

  return I;
}

cv::Mat& scanImageAndReduce_RandomAccess(cv::Mat& I, const uchar* const table) {
  CV_Assert(I.depth() == CV_8U);

  switch(I.channels()) {
    case 1: {
      for(int i = 0; i < I.rows; ++i)
        for(int j = 0; j < I.cols; ++j )
          I.at<uchar>(i,j) = table[I.at<uchar>(i,j)];
      break;
    }
    case 3: {
      cv::Mat_<cv::Vec3b> _I = I;
      for(int i = 0; i < I.rows; ++i)
        for(int j = 0; j < I.cols; ++j ) {
        _I(i,j)[0] = table[_I(i,j)[0]];
        _I(i,j)[1] = table[_I(i,j)[1]];
        _I(i,j)[2] = table[_I(i,j)[2]];
        }
      I = _I;
      break;
    }
  }

  return I;
}

