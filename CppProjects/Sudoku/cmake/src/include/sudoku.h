#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include "opencv2/imgproc/imgproc.hpp"

#include "include/digitrecognizer.h"

using namespace cv;
using namespace std;

extern const int sizex;
extern const int sizey;

/**
 * @brief drawLine
 * @param line
 * @param img
 * @param rgb
 */
void drawLine(Vec2f line, Mat &img, Scalar rgb);

/**
 * @brief mergeRelatedLines
 * @param lines
 * @param img
 */
void mergeRelatedLines(vector<Vec2f> *lines, Mat &img);

int getBox(Mat& image,Mat&  box ,Mat& Originalbox);
