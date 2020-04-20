#include <iostream>
#include <stdlib.h>
#include <stdio.h>
#include <vector>

#include <opencv2/core.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/core/utils/filesystem.hpp>

//extern const char* window_name;
//extern const char* original_window;

using namespace cv;
using namespace std;

void PCA_image(vector<String>);
void read_files(vector<String>&, String);
float meanMat(Mat);
Mat getMat(Mat, Size, double);
void createData(vector<Mat>, Mat&, Mat&);
void getOriginal(Mat, Mat, Mat&, int, int);
void plotPCA(vector<String>, int, int);
void drawAxis(Mat&, Point, Point, Scalar, const float);
