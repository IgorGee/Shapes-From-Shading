#ifndef UTILITIES_H
#define UTILITIES_H

#include <iostream>
#include <cmath>
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

bool argsS2Check(int argc) {
  if (argc != 6) {
    cout << "usage: ./s2 <input parameters file> <image 1> <image 2> <image 3> <output directions file>" << endl;
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

  int topEquiv = 0;
  int rightEquiv = 0;
  int bottomEquiv = 0;
  int leftEquiv = 0;

  for (int r = 0; r < image.rows; r++) {
    for (int c = 0; c < image.cols; c++) {
      if (image.at<uchar>(r, c)) {
        if (r < top.x) {
          top = Point(r, c);       // if higher than top
          topEquiv = 0;
        }
        if (c > right.y) {
           right = Point(r, c);   // if more right
           rightEquiv = 0;
         }
        if (r > bottom.x) {
           bottom = Point(r, c);  // if lower than bottom
           bottomEquiv = 0;
         }
        if (c < left.y) {
           left = Point(r, c);    // if more left
           leftEquiv = 0;
         }

        // count equal values to correct for true coordinates
        if (r == top.x) topEquiv++;
        if (c == right.y) rightEquiv++;
        if (r == bottom.x) bottomEquiv++;
        if (c == left.y) leftEquiv++;
      }
    }
  }

  top.y += topEquiv / 2;
  right.x += rightEquiv / 2;
  bottom.y += bottomEquiv / 2;
  left.x += leftEquiv / 2;

  vector<Point> TRBL = {top, right, bottom, left};
  return TRBL;
}

Point getBrightestPoint(Mat &image) {
  Point brightest = Point(0, 0);

  for (int r = 0; r < image.rows; r++) {
    for (int c = 0; c < image.cols; c++) {
      if (image.at<uchar>(r, c) > image.at<uchar>(brightest.x, brightest.y))
        brightest = Point(r, c);
    }
  }

  return brightest;
}

double getP(int x, int y, int r) {
  return -x / sqrt(pow(r, 2) - (pow(x, 2) + pow(y, 2)));
}

double getQ(int x, int y, int r) {
  return -y / sqrt(pow(r, 2) - (pow(x, 2) + pow(y, 2)));
}

Point3d getNormal(double p, double q) {
  p /= sqrt(pow(p, 2) + pow(q, 2) + 1);
  q /= sqrt(pow(p, 2) + pow(q, 2) + 1);
  return Point3d(p, q, 1);
}

void scaleNormalWithBrightness(Point3d &normal, int brightness) {
  double ratio = brightness / 255.0;
  normal.x *= ratio;
  normal.y *= ratio;
  normal.z *= ratio;
}

#endif
