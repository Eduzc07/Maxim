#include <opencv2/ml.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/opencv.hpp>

#include <iostream>
#include <stdio.h>
#include <fstream>

using namespace cv;
using namespace std;
using namespace cv::ml;

const int train_samples = 1;
const int classes = 10;
const int numImages = classes * train_samples;
const int sizex = 20;
const int sizey = 30;
int ImageSize = sizex * sizey;
//char pathToImages[] = "./images";
char pathToImages[] = "./images/Test2";

void PreProcessImage(Mat *inImage,Mat *outImage,int sizex, int sizey);
void LearnFromImages(int var_count, Mat* _trainData, Mat* _trainClass);
void RunSelfTest(Ptr<KNearest>& knn2);
void AnalyseImage(Ptr<KNearest>&);

/** @function main */
int main(int argc, char** argv)
{

// Mat matTrainFeatures(numImages,ImageSize,CV_32F);
// Mat matTrainLabels=Mat::zeros(numImages,1,CV_32F);
 Mat matTrainFeatures;
 Mat matTrainLabels;

 namedWindow("single", WINDOW_AUTOSIZE);
 namedWindow("all", WINDOW_AUTOSIZE);

 string fileName="save.data";

 ifstream file(fileName, ios_base::out|ios_base::in);
 if(!file) {
    cout << "save.data does not exist\n";

    LearnFromImages(ImageSize,&matTrainFeatures, &matTrainLabels);

    Ptr<KNearest> knearest=KNearest::create();
    knearest->setDefaultK(1);
    knearest->setIsClassifier(true);
// knearest->setAlgorithmType(KNearest::BRUTE_FORCE);

    Ptr<TrainData> trainingData;
    trainingData=TrainData::create(matTrainFeatures,ROW_SAMPLE,matTrainLabels);

// cout << "labels:  \n" << trainingData->getClassLabels()<<"\n";
// cout << "TrainResponses:  \n" << trainingData->getTrainResponses()<<"\n";
// cout << "samples: \n " << trainingData->getSamples()<<"\n";
// cout << "Nsamples: \n " << trainingData->getNSamples()<<"\n";

// cout << "Classificated?? \n " << knearest->isClassifier()<<" --  " <<knearest->isTrained()<<"\n";

    knearest->train(trainingData);

// cout << "Trained? \n " << knearest->isClassifier()<<" --  " <<knearest->isTrained()<<"\n";
    knearest->save(fileName);
 }else{
     cout << "save.data exists \n";
 }

 cout << "losgehts\n";

 // AnalyseImage(knearest);

 Ptr<KNearest> knn3 = StatModel::load<KNearest>(fileName);
 RunSelfTest(knn3);

 return 0;

}

void PreProcessImage(Mat *inImage,Mat *outImage,int sizex, int sizey)
{
 Mat grayImage,blurredImage,thresholdImage,contourImage,regionOfInterest;

 vector<vector<Point> > contours;

 cvtColor(*inImage,grayImage , COLOR_BGR2GRAY);

 GaussianBlur(grayImage, blurredImage, Size(5, 5), 2, 2);
 adaptiveThreshold(blurredImage, thresholdImage, 255, 1, 1, 11, 2);

 thresholdImage.copyTo(contourImage);

 findContours(contourImage, contours, RETR_LIST, CHAIN_APPROX_SIMPLE);

 int idx = 0;
 size_t area = 0;
 for (size_t i = 0; i < contours.size(); i++)
 {
  if (area < contours[i].size() )
  {
   idx = i;
   area = contours[i].size();
  }
 }

 Rect rec = boundingRect(contours[idx]);

 regionOfInterest = thresholdImage(rec);
 resize(regionOfInterest,*outImage, Size(sizex, sizey));

}

void LearnFromImages(int var_count, Mat* _trainData,Mat* _trainClasses){

   vector<int> trainClasses;

   _trainClasses->release();
   _trainData->release();

   Mat matTrainFeatures(1,var_count,CV_32F);

   Mat img;
   char file[255];
 for (int i = 0; i < classes; i++)
 {

  sprintf(file, "%s/%d.png", pathToImages, i);
    cout << file << endl;

  img = imread(file, 1);
  if (!img.data)
  {
    cout << "File " << file << " not found\n";
    exit(1);
  }

  Mat outfile;
  PreProcessImage(&img, &outfile, sizex, sizey);
  imshow("Training file",outfile);
  waitKey(0);

  for (int n = 0; n < ImageSize; n++)
  {
     matTrainFeatures.at<float>(n)=(float)outfile.data[n];
//      trainData.at<float>(i,n)=(float)outfile.data[n];

  }
     trainClasses.push_back((int)i);
//  trainClasses.at<float>(i,0)=i;
   _trainData->push_back(matTrainFeatures);
 }
   Mat(trainClasses).copyTo(*_trainClasses);
}

void RunSelfTest(Ptr<KNearest>& knn2)
{
 Mat img;
 Mat sample2 = Mat(1,ImageSize, CV_32F); //rows,cols

 // SelfTest
 char file[255];
 int z = 0;
 while (z++ < 10)
 {
  int iSecret = rand() % 10;
  //cout << iSecret;
  sprintf(file, "%s/%d.png", pathToImages, iSecret);
  img = imread(file, 1);
  Mat stagedImage;
  PreProcessImage(&img, &stagedImage, sizex, sizey);

  for (int n = 0; n < ImageSize; n++)
  {
     sample2.at<float>(n)=(float)stagedImage.data[n];//rows,cols
  }

  float detectedClass =knn2->predict(sample2);

  if (iSecret != (int) ((detectedClass)))
  {
   cout << "Falsch. Ist " << iSecret << " aber geraten ist "
     << (int) ((detectedClass));
   exit(1);
  }
  cout << "Richtig " << (detectedClass) << "\n";
  imshow("single", img);
  waitKey(0);
 }
}

void AnalyseImage(Ptr<KNearest>& knearest)
{

 Mat sample2 = Mat(1,ImageSize, CV_32F); //rows,cols
 Mat image, gray, blur, thresh;

 vector < vector<Point> > contours;
 image = imread("./images/buchstaben.png", 1);

 cvtColor(image, gray, COLOR_BGR2GRAY);
 GaussianBlur(gray, blur, Size(5, 5), 2, 2);
 adaptiveThreshold(blur, thresh, 255, 1, 1, 11, 2);
 findContours(thresh, contours, RETR_LIST, CHAIN_APPROX_SIMPLE);

 for (size_t i = 0; i < contours.size(); i++)
 {
  vector < Point > cnt = contours[i];
  if (contourArea(cnt) > 50)
  {
   Rect rec = boundingRect(cnt);
   if (rec.height > 28)
   {
    Mat roi = image(rec);
    Mat stagedImage;
    PreProcessImage(&roi, &stagedImage, sizex, sizey);
    for (int n = 0; n < ImageSize; n++){
        sample2.at<float>(n)=(float)stagedImage.data[n];//rows,cols
    }
    float result = knearest->predict(sample2);
    rectangle(image, Point(rec.x, rec.y),
      Point(rec.x + rec.width, rec.y + rec.height),
      Scalar(0, 0, 255), 2);

    imshow("all", image);
    cout << result << "\n";

    imshow("single", stagedImage);
    waitKey(0);
   }

  }

 }
}
