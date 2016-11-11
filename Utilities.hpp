#ifndef UTILITIES_H
#define UTILITIES_H

#include <iostream>
#include <fstream>
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

bool argsS3Check(int argc) {
  if (argc != 8) {
    cout << "usage: ./s3 <input directions> <image 1> <image 2> <image 3> <step> <threshold> <output>" << endl;
    return false;
  }
  return true;
}

bool argsS4Check(int argc) {
  if (argc != 7) {
    cout << "usage: ./s4 <input directions> <image 1> <image 2> <image 3> <threshold> <output>" << endl;
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
  normal.x *= brightness;
  normal.y *= brightness;
  normal.z *= brightness;
}

void fillSourceMatrix(double matrix[3][3], ifstream &directionsFile) {
  for (int i = 0; i < 9; i++)
    directionsFile >> matrix[0][i];
  directionsFile.close();
}

bool isVisisbleInAllImages(vector<Mat> images, int row, int col, int threshold) {
  for (auto image : images) {
    if (image.at<uchar>(row, col) < threshold) return false;
  }
  return true;
}

double getDeterminant(double matrix[2][2]) {
  return matrix[0][0] * matrix[0][3] - matrix[0][1] * matrix[0][2];
}

double getDeterminant(double matrix[3][3]) {
  return
  matrix[0][0] * (matrix[0][4] * matrix[0][8] - matrix[0][5] * matrix[0][7]) -
  matrix[0][1] * (matrix[0][3] * matrix[0][8] - matrix[0][5] * matrix[0][6]) +
  matrix[0][2] * (matrix[0][3] * matrix[0][7] - matrix[0][4] * matrix[0][6]);
}

void getMinorMatrixAt(int minorI, int minorJ, double matrix[3][3], double minorMatrix[2][2]) {
  int count = 0;
  for (int i = 0; i < 3; i++) {
    for (int j = 0; j < 3; j++) {
      if (count == 4) break;
      if (i != minorI && j != minorJ) {
        minorMatrix[0][count] = matrix[i][j];
        count++;
      }
    }
  }
}

void getMatrixOfMinors(double matrix[3][3], double minors[3][3]) {
  auto minorMatrix = new double[2][2];
  for (int i = 0; i < 3; i++) {
    for (int j = 0; j < 3; j++) {
      getMinorMatrixAt(i, j, matrix, minorMatrix);
      minors[i][j] = getDeterminant(minorMatrix);
    }
  }
}

void getMatrixOfCofactors(double matrix[3][3]) {
  for (int i = 1; i < 9; i+=2) {
    matrix[0][i] *= -1;
  }
}

void swap(double matrix[3][3], int a, int b) {
  double temp = matrix[a][b];
  matrix[a][b] = matrix[b][a];
  matrix[b][a] = temp;
}

void getAdjugate(double matrix[3][3]) {
  for (int i = 0; i < 3; i++)
    for (int j = i; j < 3; j++)
      if (i != j) swap(matrix, i, j);
}

void getInverseMatrix(double determinant, double matrix[3][3]) {
  for (int i = 0; i < 9; i++) {
    matrix[0][i] /= determinant;
  }
}

void markBlackWithWhiteCircle(Mat &image, int row, int col) {
  image.at<uchar>(row, col) = 0;
  image.at<uchar>(row-1, col) = 255;
  image.at<uchar>(row, col+1) = 255;
  image.at<uchar>(row+1, col) = 255;
  image.at<uchar>(row, col-1) = 255;
}

void fillIntensityMatrix(double matrix[3], vector<Mat> images, int row, int col) {
  for (int i = 0; i < 3; i++)
    matrix[i] = images[i].at<uchar>(row, col);
}

void getNormal(Point3d &normal, double inverseMatrix[3][3], double intensityMatrix[3]) {
  normal.x = inverseMatrix[0][0]*intensityMatrix[0] +
             inverseMatrix[0][1]*intensityMatrix[1] +
             inverseMatrix[0][2]*intensityMatrix[2];
  normal.y = inverseMatrix[1][0]*intensityMatrix[0] +
             inverseMatrix[1][1]*intensityMatrix[1] +
             inverseMatrix[1][2]*intensityMatrix[2];
  normal.z = inverseMatrix[2][0]*intensityMatrix[0] +
             inverseMatrix[2][1]*intensityMatrix[1] +
             inverseMatrix[2][2]*intensityMatrix[2];

  double length = sqrt(pow(normal.x, 2) + pow(normal.y, 2) + pow(normal.z, 2));
  normal.x /= length;
  normal.y /= length;
  normal.z /= length;
}

#endif
