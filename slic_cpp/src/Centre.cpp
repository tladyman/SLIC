#include "Centre.h"

Centre::Centre(int x, int y){
  this->x = x;
  this->y = y;

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

}
