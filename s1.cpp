#include "Utilities.hpp"
#include "Filters.hpp"
#include <cstdlib>
#include <iostream>
#include <fstream>
#include <vector>
#include <opencv2/opencv.hpp>

using namespace cv;
using namespace std;


int main(int argc, char *argv[]) {
  if (!argsS1Check(argc)) return -1;
  Mat image = imread(argv[1], CV_LOAD_IMAGE_GRAYSCALE);
  if (!imageValidityCheck(image)) return -1;
  CV_Assert(image.depth() == CV_8U);

  int threshold = atoi(argv[2]);
  ofstream parametersFile(argv[3]);

  Mat binaryImage;
  applyBinary(image, binaryImage, threshold);

  namedWindow("Original Image", WINDOW_AUTOSIZE);
  namedWindow("Binary Image", WINDOW_AUTOSIZE);

  imshow("Original Image", image);
  imshow("Binary Image", binaryImage);

  waitKey(0);

  parametersFile << 11 << " " << 17 << endl;
  parametersFile.close();

  return 0;
}
