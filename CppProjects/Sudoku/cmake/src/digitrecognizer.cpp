#include "include/digitrecognizer.h"

DigitRecognizer::DigitRecognizer()
{

}

DigitRecognizer::~DigitRecognizer()
{
    delete knn;
}

cv::Mat DigitRecognizer::preprocessImage(cv::Mat img)
{
    //Mat cloneImg = Mat(numRows, numCols, CV_8UC1);
      //resize(img, cloneImg, Size(numCols, numRows));

      // Try to position the given image so that the
      // character is at the center

      // Step 1: Find a good enough bounding box
      // How? Starting at the center, find the first rows and colums
      // in 4 directions such that less than 10% of the pixels are
      // bright

      int rowTop=-1, rowBottom=-1, colLeft=-1, colRight=-1;

      Mat temp;
      int thresholdBottom = 50;
      int thresholdTop = 50;
      int thresholdLeft = 50;
      int thresholdRight = 50;
      int center = img.rows/2;

      for(int i = center;i < img.rows;i++) {
          if(rowBottom == -1) {
              temp = img.row(i);
              if(sum(temp).val[0] < thresholdBottom || i==img.rows-1)
                  rowBottom = i;
          }

          if(rowTop == -1) {
              temp = img.row(img.rows-i);
              if(sum(temp).val[0] < thresholdTop || i==img.rows-1)
                  rowTop = img.rows-i;
          }

          if(colRight == -1) {
              temp = img.col(i);
              if(sum(temp).val[0] < thresholdRight|| i==img.cols-1)
                  colRight = i;
          }

          if(colLeft == -1) {
              temp = img.col(img.cols-i);
              if(sum(temp).val[0] < thresholdLeft|| i==img.cols-1)
                  colLeft = img.cols-i;
          }
      }

      // Point2i pt = Point((colLeft+colRight)/2, (rowTop+rowBottom)/2);
      /*line(img, Point(0, rowTop), Point(img.cols, rowTop), cvScalar(255,255,255));
      line(img, Point(0, rowBottom), Point(img.cols, rowBottom), cvScalar(255,255,255));
      line(img, Point(colLeft, 0), Point(colLeft, img.rows), cvScalar(255,255,255));
      line(img, Point(colRight, 0), Point(colRight, img.rows), cvScalar(255,255,255));
      imshow("Testing the image", img);
      cvWaitKey(0);*/

      // Now, position this into the center

      Mat newImg;
      newImg = newImg.zeros(img.rows, img.cols, CV_8UC1);

      int startAtX = (newImg.cols/2)-(colRight-colLeft)/2;
      int startAtY = (newImg.rows/2)-(rowBottom-rowTop)/2;

      for(int y=startAtY;y<(newImg.rows/2)+(rowBottom-rowTop)/2;y++) {
          uchar *ptr = newImg.ptr<uchar>(y);
          for(int x=startAtX;x<(newImg.cols/2)+(colRight-colLeft)/2;x++) {
              ptr[x] = img.at<uchar>(rowTop+(y-startAtY),colLeft+(x-startAtX));
          }
      }

      Mat cloneImg = Mat(numRows, numCols, CV_8UC1);
      resize(newImg, cloneImg, Size(numCols, numRows));

      // Now fill along the borders
      for(int i=0;i<cloneImg.rows;i++) {
          floodFill(cloneImg, Point(0, i), Scalar(0,0,0));
          floodFill(cloneImg, Point(cloneImg.cols-1, i), Scalar(0,0,0));

          floodFill(cloneImg, Point(i, 0), Scalar(0));
          floodFill(cloneImg, Point(i, cloneImg.rows-1), Scalar(0));
      }

//      imshow("process", cloneImg);
//      waitKey();
    cloneImg = cloneImg.reshape(1, 1);
    return cloneImg;
}

int DigitRecognizer::classify(cv::Mat img) {
    Mat cloneImg = preprocessImage(img);

    Mat cloneImg32 = Mat(1, numCols*numRows, CV_32F);
    cloneImg.convertTo(cloneImg32, CV_32F);

//    std::cout << "Sizel: "<< cloneImg32.cols << std::endl;

    float val=knn->predict(cloneImg32);

//    std::cout << "val: "<< val << std::endl;

    string fileName="../install/Character_Recognition/save.data";
    Ptr<KNearest> knn3 = StatModel::load<KNearest>(fileName);
    Mat sample2 = Mat(1,30*20, CV_32F); //rows,cols
    for (int n = 0; n < 20*30; n++)
    {
       sample2.at<float>(n)=(float)img.data[n];//rows,cols
    }

    float val2=knn3->predict(sample2 );
//    cout << "\n Train? knn3 \n " << knn3->isClassifier()<<" --  " <<knn3->isTrained() <<"\n";

    return (int)val2;
}

int DigitRecognizer::readFlippedInteger(FILE *fp)
{
    int ret = 0;

    BYTE *temp;

    temp = (BYTE*)(&ret);
    fread(&temp[3], sizeof(BYTE), 1, fp);
    fread(&temp[2], sizeof(BYTE), 1, fp);
    fread(&temp[1], sizeof(BYTE), 1, fp);
    fread(&temp[0], sizeof(BYTE), 1, fp);

    return ret;
}

bool DigitRecognizer::train(char *trainPath, char *labelsPath)
{
    FILE *fp = fopen(trainPath, "rb");
    FILE *fp2 = fopen(labelsPath, "rb");

    if(!fp || !fp2)
            return false;

   // Read bytes in flipped order
    int magicNumber = readFlippedInteger(fp);
    numImages = readFlippedInteger(fp);
    numRows = readFlippedInteger(fp);
    numCols = readFlippedInteger(fp);

//    printf("Magic number: %4x\n", magicNumber);
//    printf("Number of images: %d\n", numImages);
//    printf("Number of rows: %d\n", numRows);
//    printf("Number of columns: %d\n", numCols);

    if(numImages > MAX_NUM_IMAGES) numImages = MAX_NUM_IMAGES;

    //////////////////////////////////////////////////////////////////
    // Go through each training data entry and figure out a
    // center for each digit

    int size = numRows*numCols;

    Mat matTrainFeatures(numImages,size,CV_32F);
    Mat matTrainLabels=Mat::zeros(numImages,1,CV_32F);

//    Mat matTrainFeatures; //data
    Mat matResponses; //classes
    Mat data;
    Mat el_ptr(1, size, CV_32F);

    vector<int> responses;

//    memset( matTrainLabels.ptr(), 0, sizeof(float)*numImages;

    BYTE *temp = new BYTE[size];
    BYTE tempClass=1;

    for(int i=0;i<numImages;i++)
    {
         fread((void*)temp, size, 1, fp);
         fread((void*)(&tempClass), sizeof(BYTE), 1, fp2);
         matTrainLabels.at<float>(i,0)=tempClass;
         responses.push_back((float)tempClass);
         for(int k=0;k<size;k++){
             matTrainFeatures.at<float>(i,k)=temp[k];
             el_ptr.at<float>(k)=temp[k];
         }
         data.push_back(el_ptr);
    }
     Mat(responses).copyTo(* &matResponses);
//     std::cout <<"responses: "<<responses.size()<< std::endl;
//     std::cout <<"responses: "<< matResponses.rows<< std::endl;
//     std::cout << matResponses<< std::endl;

//std::cout <<"aqui"<<matTrainFeatures.cols<< std::endl;
//    Mat img(28,28,CV_8U);
//    for (int k=0;k<20;k++){
//        int num=0;
//        for (int i=0;i<28;i++){
//            for (int j=0;j<28;j++){
//                img.at<uchar>(i,j)=data.at<float>(k,num+28*28*k);
//                num++;
//            }
//        }

//        std::cout <<"label:"<< matResponses.at<float>(k,1)<< std::endl;
//        float n;

//        Mat imgN;
//        img.convertTo(imgN, CV_8U);
//        imshow("aaa",imgN);
//        waitKey(0);
////        destroyAllWindows();
////        std::cout << "Enter correct label: "; // no flush needed
////        std::cin >> n;

////        matTrainLabels.at<float>(k,0)=n;
//    }


    Ptr<TrainData> trainingData;
    trainingData=TrainData::create(data,ROW_SAMPLE,matResponses);
    knn->setIsClassifier(true);
    knn->setDefaultK(1);

//    cout << "labels:  \n" << trainingData->getClassLabels()<<"\n";
//    cout << "TrainResponses:  \n" << trainingData->getTrainResponses()<<"\n";
//    cout << "samples: \n " << trainingData->getSamples()<<"\n";
//    cout << "Nsamples: \n " << trainingData->getNSamples()<<"\n";

//    cout << "Classificated?? \n " << knn->isClassifier()<<" --  " <<knn->isTrained()<<"\n";

    knn->train(trainingData);

//    cout << "Trained? \n " << knn->isClassifier()<<" --  " <<knn->isTrained()<<"\n";


    fclose(fp);
    fclose(fp2);
    return true;
}
