#include "Utilities.hpp"
#include "Filters.hpp"
#include <cstdlib>
#include <iostream>
#include <string>
#include <fstream>
#include <opencv2/opencv.hpp>

using namespace cv;
using namespace std;


int main(int argc, char *argv[]) {
  const int NEEDLE_LENGTH = 10;

  if (!argsS3Check(argc)) return -1;

  ifstream directionsFile(argv[1]);
  vector<string> imageFiles = {argv[2], argv[3], argv[4]};
  int step = atoi(argv[5]);
  int threshold = atoi(argv[6]);

  auto sourceMatrix = new double[3][3];
  auto inverseMatrix = new double[3][3];
  fillSourceMatrix(sourceMatrix, directionsFile);

  double sourceDeterminant = getDeterminant(sourceMatrix);
  getMatrixOfMinors(sourceMatrix, inverseMatrix);
  getMatrixOfCofactors(inverseMatrix);
  getAdjugate(inverseMatrix);
  getInverseMatrix(sourceDeterminant, inverseMatrix);

  vector<Mat> images;
  for (auto imageName : imageFiles) {
    Mat image = imread(imageName, CV_LOAD_IMAGE_GRAYSCALE);
    if (!imageValidityCheck(image)) return -1;
    CV_Assert(image.depth() == CV_8U);
    images.push_back(image);
  }

  Mat needleImage = images[0].clone();
  auto intensityMatrix = new double[3];
  Point3d normal;

  for (int r = NEEDLE_LENGTH; r < needleImage.rows - NEEDLE_LENGTH; r += step) {
    for (int c = NEEDLE_LENGTH; c < needleImage.cols - NEEDLE_LENGTH; c += step) {
      bool inAll3 = true;
      for (auto image : images)
        if (image.at<uchar>(r, c) < threshold) inAll3 = false;
      if (!inAll3) continue;
      fillIntensityMatrix(intensityMatrix, images, r, c);
      getNormal(normal, inverseMatrix, intensityMatrix);
      double newR = r + normal.x * 10;
      double newC = c + normal.y * 10;
      line(needleImage, Point(c, r), Point(newC, newR), 255);
      markBlackWithWhiteCircle(needleImage, r, c);
    }
  }

  namedWindow("Original Image", WINDOW_AUTOSIZE);
  namedWindow("Needle Image", WINDOW_AUTOSIZE);

  imshow("Original Image", images[0]);
  imshow("Needle Image", needleImage);

  waitKey(0);

  vector<int> params = { CV_IMWRITE_PXM_BINARY };
  imwrite(argv[7], needleImage, params);

  return 0;
}
