#ifndef SLIC_H
#define SLIC_H

#include <iostream>
#include <string>
#include <vector>
#include "opencv2/opencv.hpp"
#include "opencv2/highgui/highgui.hpp"

#include "Centre.h"

using namespace std;
using namespace cv;

class Slic{
public:
  Slic(string inputFile, int k);
  ~Slic();

  void drawCentres();
  Mat calculateGradientImage();
private:
  int k;
  double S, vSpacing;

  string inputFile;
  Mat image, greyImage;

  vector<Centre*> centres;
};

#endif // SLIC_H
