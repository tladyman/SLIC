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
  Slic(string inputFile, string outputLoc, int k, double m, bool stages = true, bool saveOutput = true, bool showGui = true);
  ~Slic();

  Mat drawCentres(bool gui = false);
  Mat drawLowResolution(bool gui = false);
  Mat drawBoundaries(bool gui = false);
  Mat calculateGradientImage();
  void iterate();
private:
  int k;
  double S, vSpacing, m;
  string outputFolder;

  string inputFile;
  Mat image, greyImage;

  vector<Centre*> centres;
  Pixel** pixelGrid;
};

#endif // SLIC_H
