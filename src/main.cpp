#include <cstdlib>
#include <cstdio>
#include <format>

#include <opencv2/core.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>

int main(int argc, char** argv) {
  if(argc != 2) {
    printf("%s", std::format("Usage example:\t{} img/exampleImage.png", argv[0]).c_str());
    return EXIT_FAILURE;
  }

  cv::Mat image;
  image = imread(argv[1], cv::IMREAD_COLOR); // Read the file

  // Check for invalid input
  if(image.empty()) {
    printf("Could not open or find the image");
    return EXIT_FAILURE;
  }

  namedWindow("Display window", cv::WINDOW_AUTOSIZE); // Create a window for display.
  imshow("Display window", image ); // Show our image inside it.

  cv::waitKey(0); // Wait for a keystroke in the window

  return EXIT_SUCCESS;
}

