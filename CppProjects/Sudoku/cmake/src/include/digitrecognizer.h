
#include <opencv2/ml.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/opencv.hpp>

#define MAX_NUM_IMAGES    60000
using namespace cv;
using namespace std;
using namespace cv::ml;
typedef unsigned char BYTE;

class DigitRecognizer {
public:
  DigitRecognizer();
	~DigitRecognizer();
	bool train(char* trainPath, char* labelsPath);
	int classify(Mat img);

private:
        Mat preprocessImage(Mat img);
	int readFlippedInteger(FILE *fp);
private:
        Ptr<KNearest> knn=KNearest::create();
	int numRows, numCols, numImages;
};
