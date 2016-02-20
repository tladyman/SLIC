#ifndef SLIC_H
#define SLIC_H

#include <iostream>
#include <string>
#include <vector>
#include "opencv2/opencv.hpp"
#include "opencv2/highgui/highgui.hpp"

#include "Centre.h"
#include "Pixel.h"

using namespace std;
using namespace cv;

class Slic{
public:
  Slic(string inputFile, int k, double m);
  ~Slic();

  void drawCentres();
  Mat calculateGradientImage();
  void iterate();
private:
  int k;
  double S, vSpacing, m;

  string inputFile;
  Mat image, greyImage;

  vector<Centre*> centres;
  Pixel** pixelGrid;
};

#endif // SLIC_H