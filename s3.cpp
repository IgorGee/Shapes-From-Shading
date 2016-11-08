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
  if (!argsS3Check(argc)) return -1;

  ifstream directionsFile(argv[1]);
  vector<string> imageFiles = {argv[2], argv[3], argv[4]};
  int step = atoi(argv[5]);
  int threshold = atoi(argv[6]);
  ofstream needleOutputFile(argv[7]);

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

  return 0;
}
