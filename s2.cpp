#include "Utilities.hpp"
#include "Filters.hpp"
#include <iostream>
#include <string>
#include <fstream>
#include <opencv2/opencv.hpp>

using namespace cv;
using namespace std;


int main(int argc, char *argv[]) {
  if (!argsS2Check(argc)) return -1;

  ifstream parametersFile(argv[1]);
  vector<string> imageFiles = {argv[2], argv[3], argv[4]};
  ofstream directionsFile(argv[5]);

  int center_row, center_col, radius;
  parametersFile >> center_row >> center_col >> radius;

  for (auto imageName : imageFiles) {
    Mat image = imread(imageName, CV_LOAD_IMAGE_GRAYSCALE);
    if (!imageValidityCheck(image)) return -1;
    CV_Assert(image.depth() == CV_8U);

    Point lightSource = getBrightestPoint(image);
    cout << lightSource << endl;
  }

  return 0;
}
