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
  if (!argsS4Check(argc)) return -1;

  ifstream directionsFile(argv[1]);
  vector<string> imageFiles = {argv[2], argv[3], argv[4]};
  int threshold = atoi(argv[5]);

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

  Mat albedoImage = images[0].clone();
  auto intensityMatrix = new double[3];
  Point3d normal;

  for (int r = 0; r < albedoImage.rows; r++) {
    for (int c = 0; c < albedoImage.cols; c++) {
      bool inAll3 = true;
      for (auto image : images)
        if (image.at<uchar>(r, c) < threshold) inAll3 = false;
      if (!inAll3) {
        albedoImage.at<uchar>(r, c) = 0;
        continue;
      }
      fillIntensityMatrix(intensityMatrix, images, r, c);
      double albedo = getAlbedo(normal, inverseMatrix, intensityMatrix);
      cout << albedo << endl;
      albedoImage.at<uchar>(r, c) = albedoImage.at<uchar>(r,c)/2 * albedo;
    }
  }

  namedWindow("Original Image", WINDOW_AUTOSIZE);
  namedWindow("Albedo Image", WINDOW_AUTOSIZE);

  imshow("Original Image", images[0]);
  imshow("Albedo Image", albedoImage);

  waitKey(0);

  vector<int> params = { CV_IMWRITE_PXM_BINARY };
  imwrite(argv[6], albedoImage, params);

  return 0;
}
