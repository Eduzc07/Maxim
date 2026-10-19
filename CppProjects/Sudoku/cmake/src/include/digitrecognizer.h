#ifndef DIGITRECOGNIZER_H
#define DIGITRECOGNIZER_H

#include <opencv2/ml.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/opencv.hpp>
#include <string>

#define MAX_NUM_IMAGES 60000
using namespace cv;
using namespace std;
using namespace cv::ml;
typedef unsigned char BYTE;

class DigitRecognizer {
public:
    DigitRecognizer();
    ~DigitRecognizer();
    bool train(const char* trainPath, const char* labelsPath);
    bool loadModel(const std::string& modelPath);
    int classify(const Mat& img, int* secondBest = nullptr, float* confidenceMargin = nullptr);

private:
    Mat preprocessImage(Mat img);
    int readFlippedInteger(FILE *fp);

private:
    Ptr<KNearest> knn;
    Ptr<KNearest> knnPrinted;
    int numRows = 28;
    int numCols = 28;
    int numImages = 0;
};

#endif // DIGITRECOGNIZER_H

