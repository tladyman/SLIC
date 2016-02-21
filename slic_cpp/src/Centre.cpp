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
