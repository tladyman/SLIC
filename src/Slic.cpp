#include "Slic.h"

Slic::Slic(string inputFile, int k){
  this->inputFile = inputFile;

  // Load the image
  this->image = imread(inputFile);
  if(!image.data ) {
    cout <<  "Could not open or find the image" << endl ;
  }

  // Create a grey version for magnitude etc
  cvtColor(image, greyImage, CV_BGR2GRAY);

  // Convert to LAB colourspace to enable
  // distance between colour to be Euclidean
  // distance
  cvtColor(this->image, this->image, CV_BGR2Lab);

  // Place k superpixels evenly over the image
  //
  // Spacing between superpixels assuming hexagonal grid
  // A lot of this section from:
  // http://www.peterkovesi.com/matlabfns/Spatial/slic.m
  this->S = sqrt( (image.cols * image.rows) / (k*sqrt(3)/2));
  // Centres per row allowing 0.5 S spacing at one edge.
  // Edge alternates
  int centresPerRow = round(image.rows/(sqrt(3)/2*S));
  // Given an integer number of nodes per row recompute S
  S = image.cols/(centresPerRow + 0.5);
  // Get number of rows of centres assuming the same hexagonal
  // structure and spacing
  int nCentreRows = round(image.rows/(sqrt(3)/2*S));
  this->vSpacing = image.rows/nCentreRows;
  // Recompute k so that the structure is posisble
  this->k = nCentreRows * centresPerRow;

  // Create the centres:
  for(int j=0; j<nCentreRows; j++){
    for(int i=0; i<centresPerRow; i++){
      centres.push_back(new Centre(((j%2)*S) + (i*S), S+(j*vSpacing)));
    }
  }

  this->drawCentres();

  // Calculate the gradient image to reposition the centres slightly
  Mat gradient = this->calculateGradientImage();

  // Show the gradient
  namedWindow( "Gradient", CV_WINDOW_AUTOSIZE );
  imshow( "Gradient", greyImage );
  waitKey(0);
  imshow( "Gradient", gradient );
  waitKey(0);
}

void Slic::drawCentres(){
  Mat markedImage = image.clone();

  for(int i=0; i<centres.size();i++){
    circle(markedImage, Point(centres[i]->x, centres[i]->y),S/2, Scalar(255,255,255));
  }

  // Show the image
  cvtColor(markedImage, markedImage, CV_Lab2BGR);
  namedWindow( "Centres", CV_WINDOW_AUTOSIZE );
  imshow( "Centres", markedImage );

  waitKey(0);
}

Mat Slic::calculateGradientImage(){
  // Set up the kernels
  Mat kernelH, kernelV;
  kernelH = Mat::zeros( 3, 3, CV_32F );
  kernelV = Mat::zeros( 3, 3, CV_32F );
  kernelH.at<Vec3b>(Point(1,0)) = -1;
  kernelH.at<Vec3b>(Point(1,2)) = 1;
  kernelV.at<Vec3b>(Point(0,1)) = -1;
  kernelV.at<Vec3b>(Point(2,1)) = 1;
  // (-1,-1) anchor is centre, -1 ddepth is same as source
  Point anchor = Point(-1, -1);
  int ddepth = -1;
  double delta = 0;
  // Perform the filters
  Mat dy = this->greyImage.clone();
  Mat dx = this->greyImage.clone();
  filter2D(this->greyImage, dy, ddepth , kernelV, anchor, delta, BORDER_DEFAULT );
  filter2D(this->greyImage, dx, ddepth , kernelH, anchor, delta, BORDER_DEFAULT );
  // Create the magnitude image
  Mat mag = this->greyImage.clone();

  dy.convertTo(dy, CV_32F);
  dx.convertTo(dx, CV_32F);
  mag.convertTo(mag, CV_32F);

  cout<<"dy: " << dy.size()<< " " << dy.type() <<endl;
  cout<<"dx: " << dx.size()<< " " << dx.type()<<endl;
  cout<<"mag: " << mag.size()<< " " << mag.type()<< " " << countNonZero(mag)<<endl;

  magnitude(dy, dx, mag);
  double min, max;
  minMaxLoc(mag, &min, &max);
  mag = 255*(mag/max);
  return mag;
}

Slic::~Slic(){
  for(int i=0; i<centres.size();i++){
    delete centres[i];
  }
}
