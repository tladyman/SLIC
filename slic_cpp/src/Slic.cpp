#include "Slic.h"

Slic::Slic(string inputFile, string outputLoc, int k, double m, bool stages, bool saveOutput, bool showGui){
  this->outputFolder = outputLoc + "/";

  this->m = m;
  this->inputFile = inputFile;

  // Load the image
  this->image = imread(inputFile);
  if(!image.data ) {
    cout <<  "Could not open or find the image" << endl ;
  }

  // Show the input image
  if(showGui){
    namedWindow( "Input", CV_WINDOW_AUTOSIZE );
    imshow( "Input", image );
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
  //int centresPerRow = round(image.rows/(sqrt(3)/2*S));
  int centresPerRow = round(image.cols/S - 0.5);
  // Given an integer number of nodes per row recompute S
  S = image.cols/(centresPerRow + 0.5);
  // Get number of rows of centres assuming the same hexagonal
  // structure and spacing
  int nCentreRows = round(image.rows/(sqrt(3)/2*S));
  this->vSpacing = image.rows/nCentreRows;
  // Recompute k so that the structure is posisble
  this->k = nCentreRows * centresPerRow;
  cout<<"k: "<<this->k<<endl;

  // Create the centres:
  for(int j=0; j<nCentreRows; j++){
    for(int i=0; i<centresPerRow; i++){
      centres.push_back(new Centre(((j%2)*S) + (i*S), S+(j*vSpacing), image.at<cv::Vec3b>(i,j)[0], image.at<cv::Vec3b>(i,j)[1], image.at<cv::Vec3b>(i,j)[2]));
    }
  }

  // Save the initial centres
  if( (showGui || saveOutput) && stages) {
    Mat initialCentres = this->drawCentres(showGui);
    if(saveOutput){
      imwrite(outputFolder + "initialCentres.png", initialCentres);
    }
  }

  // Calculate the gradient image to reposition the centres slightly
  Mat gradient = this->calculateGradientImage();
  for(int i=0; i<centres.size();i++){
    centres[i]->reposition(gradient);
  }

  // Save the wiggled centres
  if( (showGui || saveOutput) && stages) {
    Mat secondCentres = this->drawCentres(showGui);
    if(saveOutput){
      imwrite(outputFolder + "secondCentres.png", secondCentres);
    }
  }

  // Save the gradient
  if( saveOutput && stages) {
    imwrite(outputFolder + "gradient.png", gradient);
  }

  // Read the pixel values into the class
  pixelGrid = new Pixel*[image.rows];
  for(int i = 0; i < image.rows; ++i) {
    pixelGrid[i] = new Pixel[image.cols];
  }

  for(int i = 0; i < image.rows; i++){
    for(int j = 0; j < image.cols; j++){
      pixelGrid[i][j].l = image.at<cv::Vec3b>(i,j)[0];
      pixelGrid[i][j].a = image.at<cv::Vec3b>(i,j)[1];
      pixelGrid[i][j].b = image.at<cv::Vec3b>(i,j)[2];

      pixelGrid[i][j].x = j;
      pixelGrid[i][j].y = i;

      pixelGrid[i][j].S = S;
      pixelGrid[i][j].m = m;
    }
  }

  // TODO: Initialise centre colours in the 2S grid
  // for(int i=0; i<centres.size();i++){
  //   centres[i]->initialiseColour(pixelGrid, image.rows, image.cols);
  // }


  // Iterate
  int nIterations = 10;
  for(int i = 0; i<nIterations; i++){
    this->iterate();
    // Save the images
    if( ((showGui || saveOutput) && stages) || (showGui && (i==nIterations-1)) ) {
      Mat iteration = this->drawLowResolution();
      Mat boundary = this->drawBoundaries();

      if(showGui){
        namedWindow( "Low Resolution", CV_WINDOW_AUTOSIZE );
        imshow( "Low Resolution", iteration );

        namedWindow( "Boundaries", CV_WINDOW_AUTOSIZE );
        imshow( "Boundaries", boundary );

        waitKey(0);
      }
      if(saveOutput){
        imwrite(outputFolder + ("iteration_" + to_string(i) + ".png"), iteration);
        imwrite(outputFolder + ("boundary_" + to_string(i) + ".png"), boundary);
      }
    }
  }

}

Mat Slic::drawCentres(bool gui){
  Mat markedImage = image.clone();

  for(int i=0; i<centres.size();i++){
    circle(markedImage, Point(centres[i]->x, centres[i]->y),S/2, Scalar(255,255,255));
  }
  if(gui){
    // Show the image
    //cvtColor(markedImage, markedImage, CV_Lab2BGR);
    namedWindow( "Centres", CV_WINDOW_AUTOSIZE );
    imshow( "Centres", markedImage );

    waitKey(0);
  }
  cvtColor(markedImage, markedImage, CV_Lab2BGR);

  return markedImage;
}

Mat Slic::drawLowResolution(bool gui){
  Mat markedImage = image.clone();

  for(int i=0; i<centres.size();i++){
    for(int j=0; j<centres[i]->pixels.size();j++){
      markedImage.at<cv::Vec3b>(Point(centres[i]->pixels[j]->x,centres[i]->pixels[j]->y))[0] = (int)centres[i]->l;
      markedImage.at<cv::Vec3b>(Point(centres[i]->pixels[j]->x,centres[i]->pixels[j]->y))[1] = (int)centres[i]->a;
      markedImage.at<cv::Vec3b>(Point(centres[i]->pixels[j]->x,centres[i]->pixels[j]->y))[2] = (int)centres[i]->b;
    }
  }
  cvtColor(markedImage, markedImage, CV_Lab2BGR);
  if(gui){
    // Show the image
    namedWindow( "Super Pixels", CV_WINDOW_AUTOSIZE );
    imshow( "Super Pixels", markedImage );

    waitKey(0);
  }

  return markedImage;
}

Mat Slic::drawBoundaries(bool gui){
  Mat markedImage = this->drawLowResolution();

  for(int i = 0; i < image.rows; i++){
    for(int j = 0; j < image.cols; j++){
      Centre* thisCentre = pixelGrid[i][j].currentCentre;
      if((i-1 > 0) &&(j-1 > 0)) {
        if((pixelGrid[i-1][j-1].currentCentre != thisCentre) || (pixelGrid[i-1][j].currentCentre != thisCentre) || (pixelGrid[i][j-1].currentCentre != thisCentre)){
          markedImage.at<cv::Vec3b>(Point(j,i))[0] = 0;
          markedImage.at<cv::Vec3b>(Point(j,i))[1] = 0;
          markedImage.at<cv::Vec3b>(Point(j,i))[2] = 0;
        }
      }
    }
  }

  //cvtColor(markedImage, markedImage, CV_Lab2BGR);
  if(gui){
    // Show the image
    //cvtColor(markedImage, markedImage, CV_Lab2BGR);
    namedWindow( "Super Pixels", CV_WINDOW_AUTOSIZE );
    imshow( "Super Pixels", markedImage );

    waitKey(0);
  }

  return markedImage;
}

Mat Slic::calculateGradientImage(){
  // Set up the kernels
  Mat kernelH, kernelV;
  kernelH = Mat::zeros( 3, 3, CV_32F );
  kernelV = Mat::zeros( 3, 3, CV_32F );
  kernelH.at<float>(Point(1,0)) = -1;
  kernelH.at<float>(Point(1,2)) = 1;
  kernelV.at<float>(Point(0,1)) = -1;
  kernelV.at<float>(Point(2,1)) = 1;
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

  magnitude(dy, dx, mag);
  double min, max;
  minMaxLoc(mag, &min, &max);
  mag = 255*(mag/max);

  return mag;
}

void Slic::iterate(){
  // Remove old assignments
  for(int i=0; i<centres.size();i++){
    centres[i]->clearPixels();
  }

  // Assignment stage
  // For each centre
  for(int i=0; i<centres.size();i++){
    // Search the 2S by 2S region around the centre
    for(int y = centres[i]->y - (2*S); y < centres[i]->y + (2*S); y++){
      // Constrained by image size
      if(y<0){
        y = 0;
      }
      if(y >= image.rows){
        break;
      }
      for(int x = centres[i]->x - (2*S); x < centres[i]->x + (2*S); x++){
        // Constrained by image size
        if(x<0){
          x = 0;
        }
        if(x > image.cols){
          break;
        }
        // Calculate distance from this point to the centre
        // and update if less
        double distance = pixelGrid[y][x].distanceToCentre(centres[i]);
        if(distance <  pixelGrid[y][x].d){
          pixelGrid[y][x].d = distance;
          pixelGrid[y][x].currentCentre = centres[i];
        }
      }
    }
  }

  // Update the centre lists
  for(int y = 0; y < image.rows; y++){
    for(int x = 0; x < image.cols; x++){
      pixelGrid[y][x].addToCentre();
    }
  }
  // Update stage
  // Update centres
  for(int i=0; i<centres.size();i++){
    centres[i]->update();
  }
  // Compute Error, E
}

Slic::~Slic(){
  for(int i=0; i<centres.size();i++){
    delete centres[i];
  }
  for(int i = 0; i < image.rows; ++i) {
    delete [] pixelGrid[i];
  }
  delete [] pixelGrid;
}
