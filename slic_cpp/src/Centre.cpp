#include "Centre.h"

Centre::Centre(int x, int y, int l, int a, int b){
  this->x = x;
  this->y = y;

  this->l = 0;
  this->a = 0;
  this->b = 0;

  this->startX = x;
  this->startY = y;
}

Centre::~Centre(){

}

void Centre::reset(){
  this->x = startX;
  this->y = startY;
}

void Centre::reposition(Mat gradientImage, int n){
  int halfKernel = ((n+1)/2);
  int startX = x - halfKernel;
  int startY = y - halfKernel;
  int stopX = x + halfKernel;
  int stopY = y + halfKernel;
  // Bounds
  if (startX<0){
    startX = 0;
  } else if (startX > gradientImage.cols){
    startX = gradientImage.cols;
  }
  if (startY<0){
    startY = 0;
  } else if (startY > gradientImage.rows){
    startY = gradientImage.rows;
  }
  if (stopX<0){
    stopX = 0;
  } else if (stopX > gradientImage.cols){
    stopX = gradientImage.cols;
  }
  if (stopY<0){
    stopY = 0;
  } else if (stopY > gradientImage.rows){
    stopY = gradientImage.rows;
  }
  // Find lowest value pixel in nxn region
  float minValue = gradientImage.at<float>(Point(x,y));
  int currentX = x;
  int currentY = y;
  for(int i = startY; i<stopY; i++){
    for(int j = startX; j<stopX; j++){
      float value = gradientImage.at<float>(Point(j,i));
      if(value < minValue){
        minValue = value;
        currentX = j;
        currentY = i;
      }
    }
  }
  // Update the centre
  x = currentX;
  y = currentY;
}

void Centre::addPixel(Pixel* pixel){
  pixels.push_back(pixel);
}

void Centre::clearPixels(){
  pixels.clear();
}

void Centre::update(){
  long long sumL, sumA, sumB, sumX, sumY;
  int N = pixels.size();
  for(int i=0; i<N;i++){
    sumL += pixels[i]->l;
    sumA += pixels[i]->a;
    sumB += pixels[i]->b;
    sumX += pixels[i]->x;
    sumY += pixels[i]->y;
  }
  if(N != 0) {
    sumL = sumL /N;
    sumA = sumA /N;
    sumB = sumB /N;
    sumX = sumX /N;
    sumY = sumY /N;
  } else {
    sumL = 0;
    sumA = 0;
    sumB = 0;
    sumX = 0;
    sumY = 0;
  }
  l = sumL;
  a = sumA;
  b = sumB;
}
