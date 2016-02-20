#include "Pixel.h"

Pixel::Pixel(int x, int y, int l, int a, int b, double S, double m){
  this->x = x;
  this->y = y;
  this->l = l;
  this->a = a;
  this->b = b;

  this->d = 100000;
  this->centre = NULL;

  this->S = S;
  this->m = m;
}

Pixel::Pixel(){
  this->d = 100000;
  this->centre = NULL;
}

Pixel::~Pixel(){

}

double Pixel::distanceToCentre(Centre* centre){
  double dc = sqrt( pow(centre->l - l,2) + pow(centre->a - a,2) + pow(centre->b - b,2) );
  double ds = sqrt( pow(centre->x - l,2) + pow(centre->y - a,2) );

  double D = sqrt( pow(dc,2) + pow(ds/S,2)*pow(m,2) );
  return D;
}
