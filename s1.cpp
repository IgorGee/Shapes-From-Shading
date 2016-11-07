#include "Utilities.hpp"
#include "Filters.hpp"
#include <cstdlib>
#include <iostream>
#include <fstream>
#include <vector>
#include <cmath>
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

  vector<Point> TRBLArray = getTRBLArrayOfSphere(binaryImage);
  Point top = TRBLArray[0];
  Point right = TRBLArray[1];
  Point bot = TRBLArray[2];
  Point left = TRBLArray[3];

  // Center row == avg(right_row, left_row)
  int center_row = (right.x + left.x) / 2;
  // Center col == avg(top_col, bot_col)
  int center_col = (top.y + bot.y) / 2;
  // radius == avg(diameters) / 2
  int verticalDiameter = sqrt(pow(top.x - bot.x, 2) + pow(top.y - bot.y, 2));
  int horizontalDiameter = sqrt(pow(left.x - right.x, 2) + pow(left.y - right.y, 2));
  int radius = (verticalDiameter + horizontalDiameter) / 2 / 2;

  namedWindow("Original Image", WINDOW_AUTOSIZE);
  namedWindow("Binary Image", WINDOW_AUTOSIZE);

  imshow("Original Image", image);
  imshow("Binary Image", binaryImage);

  waitKey(0);

  parametersFile << center_row << " " << center_col << " " << radius << endl;
  parametersFile.close();

  return 0;
}
