#ifndef PIXEL_H
#define PIXEL_H

#include <iostream>
#include <string>
#include <vector>
#include "opencv2/opencv.hpp"
#include "opencv2/highgui/highgui.hpp"

#include "Centre.h"

using namespace std;
using namespace cv;

class Pixel{
public:
  Pixel(int x, int y, int l, int a, int b, double S, double m);
  Pixel(int x, int y);
  ~Pixel();

  // Distance measure to centre
  double distanceToCentre(Centre* centre);

  int l,a,b,x,y;
  double d, S, m;
  Centre* centre;
};

#endif // PIXEL_H
