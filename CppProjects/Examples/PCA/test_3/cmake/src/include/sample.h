#include <iostream>
#include <stdlib.h>
#include <stdio.h>
#include <vector>
#include <math.h>
#include <opencv2/core.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/core/utils/filesystem.hpp>

using namespace cv;
using namespace std;

void PCA_image(String);
void read_files(vector<String> &filenames, String folder);
float meanMat(Mat);
Mat getMat(Mat,Size,double);
void createData(vector<Mat>,Mat&,Mat&);
void getOriginal(Mat,Mat,Mat&,int,int);
void plotPCA(vector<String>, int, int);
void drawAxis(Mat&, Point, Point, Scalar, const float);
void newColor(double, Scalar&);
