#ifndef CENTRE_H
#define CENTRE_H

#include <iostream>
#include <string>
#include <vector>
#include "opencv2/opencv.hpp"
#include "opencv2/highgui/highgui.hpp"

#include "Pixel.h"

using namespace std;
using namespace cv;

// Forward declaration for circular dependency
class Pixel;

class Centre{
public:
  Centre(int x, int y, int l=0, int a=0, int b=0);
  ~Centre();

  // Reposition within an NxN region to lowest gradient.
  // Ensures not on an edge
  void reposition(Mat gradientImage, int n=3);

  // Reset to the original position to avoid recomputation
  void reset();

  // Handle the list of child pixels
  void addPixel(Pixel* pixel);
  void clearPixels();
  void update();

  double l,a,b,x,y;

  double startX, startY;

  vector<Pixel*> pixels;
};

#endif // CENTRE_H
