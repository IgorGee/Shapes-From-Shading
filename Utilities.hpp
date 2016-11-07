#ifndef UTILITIES_H
#define UTILITIES_H

#include <iostream>
#include <opencv2/opencv.hpp>

using namespace std;
using namespace cv;

bool argsS1Check(int argc) {
  if (argc != 4) {
    cout << "usage: ./s1 <input original image> <threshold value> <output parameters file>" << endl;
    return false;
  }
  return true;
}

bool imageValidityCheck(Mat &image) {
  if (!image.data) {
    cout << "No image data " << endl;
    return false;
  }
  return true;
}

vector<Point> getTRBLArrayOfSphere(Mat &image) {
  Point top = Point(image.rows - 1, image.cols - 1);
  Point right = Point(image.rows - 1, 0);
  Point bottom = Point(0, 0);
  Point left = Point(0, image.cols - 1);

  for (int r = 0; r < image.rows; r++) {
    for (int c = 0; c < image.cols; c++) {
      if (image.at<uchar>(r, c)) {
        if (r < top.x) top = Point(r, c);       // if higher than top
        if (c > right.y) right = Point(r, c);   // if more right
        if (r > bottom.x) bottom = Point(r, c); // if lower than bottom
        if (c < left.y) left = Point(r, c);     // if more left
      }
    }
  }

  vector<Point> TRBL = {top, right, bottom, left};
  return TRBL;
}

#endif
