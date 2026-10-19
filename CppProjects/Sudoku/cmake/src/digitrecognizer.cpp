#include "include/digitrecognizer.h"

DigitRecognizer::DigitRecognizer() : knn(KNearest::create())
{
    loadModel("../install/Character_Recognition/save.data");
}

DigitRecognizer::~DigitRecognizer()
{
}

bool DigitRecognizer::loadModel(const std::string& modelPath)
{
    try {
        knnPrinted = StatModel::load<KNearest>(modelPath);
        return (knnPrinted != nullptr && !knnPrinted->empty());
    } catch (const cv::Exception&) {
        knnPrinted.release();
        return false;
    }
}

cv::Mat DigitRecognizer::preprocessImage(cv::Mat img)
{
    int rowTop = -1, rowBottom = -1, colLeft = -1, colRight = -1;

    Mat temp;
    int thresholdBottom = 50;
    int thresholdTop = 50;
    int thresholdLeft = 50;
    int thresholdRight = 50;
    int centerRow = img.rows / 2;
    int centerCol = img.cols / 2;

    for (int i = centerRow; i < img.rows; i++) {
        if (rowBottom == -1) {
            temp = img.row(i);
            if (sum(temp).val[0] < thresholdBottom || i == img.rows - 1)
                rowBottom = i;
        }
        if (rowTop == -1) {
            int r = img.rows - 1 - (i - centerRow);
            if (r < 0) r = 0;
            temp = img.row(r);
            if (sum(temp).val[0] < thresholdTop || r == 0)
                rowTop = r;
        }
    }

    for (int i = centerCol; i < img.cols; i++) {
        if (colRight == -1) {
            temp = img.col(i);
            if (sum(temp).val[0] < thresholdRight || i == img.cols - 1)
                colRight = i;
        }
        if (colLeft == -1) {
            int c = img.cols - 1 - (i - centerCol);
            if (c < 0) c = 0;
            temp = img.col(c);
            if (sum(temp).val[0] < thresholdLeft || c == 0)
                colLeft = c;
        }
    }

    Mat newImg = Mat::zeros(img.rows, img.cols, CV_8UC1);

    int startAtX = (newImg.cols / 2) - (colRight - colLeft) / 2;
    int startAtY = (newImg.rows / 2) - (rowBottom - rowTop) / 2;

    for (int y = startAtY; y < (newImg.rows / 2) + (rowBottom - rowTop) / 2; y++) {
        if (y < 0 || y >= newImg.rows) continue;
        uchar *ptr = newImg.ptr<uchar>(y);
        for (int x = startAtX; x < (newImg.cols / 2) + (colRight - colLeft) / 2; x++) {
            if (x < 0 || x >= newImg.cols) continue;
            int srcY = rowTop + (y - startAtY);
            int srcX = colLeft + (x - startAtX);
            if (srcY >= 0 && srcY < img.rows && srcX >= 0 && srcX < img.cols) {
                ptr[x] = img.at<uchar>(srcY, srcX);
            }
        }
    }

    Mat cloneImg = Mat(numRows, numCols, CV_8UC1);
    resize(newImg, cloneImg, Size(numCols, numRows));

    // Fill along the borders
    for (int i = 0; i < cloneImg.rows; i++) {
        floodFill(cloneImg, Point(0, i), Scalar(0, 0, 0));
        floodFill(cloneImg, Point(cloneImg.cols - 1, i), Scalar(0, 0, 0));
    }
    for (int i = 0; i < cloneImg.cols; i++) {
        floodFill(cloneImg, Point(i, 0), Scalar(0));
        floodFill(cloneImg, Point(i, cloneImg.rows - 1), Scalar(0));
    }

    cloneImg = cloneImg.reshape(1, 1);
    return cloneImg;
}

int DigitRecognizer::classify(const cv::Mat& img, int* secondBest, float* confidenceMargin)
{
    if (img.empty()) return 0;

    if (!knnPrinted || knnPrinted->empty()) {
        loadModel("../install/Character_Recognition/save.data");
    }

    Mat resizedImg;
    if (img.rows != 30 || img.cols != 20) {
        resize(img, resizedImg, Size(20, 30));
    } else {
        resizedImg = img;
    }

    Mat sample2 = Mat(1, 30 * 20, CV_32F);
    for (int n = 0; n < 20 * 30; n++) {
        sample2.at<float>(n) = (float)resizedImg.data[n];
    }

    if (knnPrinted && !knnPrinted->empty()) {
        Mat results, neighborResponses, dists;
        knnPrinted->findNearest(sample2, 2, results, neighborResponses, dists);
        int best = (int)neighborResponses.at<float>(0, 0);
        int runnerUp = (neighborResponses.cols > 1) ? (int)neighborResponses.at<float>(0, 1) : best;
        float d1 = dists.at<float>(0, 0);
        float d2 = (dists.cols > 1) ? dists.at<float>(0, 1) : d1;

        if (secondBest) *secondBest = runnerUp;
        if (confidenceMargin) *confidenceMargin = (d2 - d1);
        return best;
    }

    if (knn && knn->isTrained()) {
        Mat cloneImg = preprocessImage(img);
        Mat cloneImg32;
        cloneImg.convertTo(cloneImg32, CV_32F);
        float val = knn->predict(cloneImg32);
        if (secondBest) *secondBest = (int)val;
        if (confidenceMargin) *confidenceMargin = 0.0f;
        return (int)val;
    }

    return 0;
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

bool DigitRecognizer::train(const char *trainPath, const char *labelsPath)
{
    if (knnPrinted && !knnPrinted->empty()) {
        return true;
    }

    FILE *fp = fopen(trainPath, "rb");
    FILE *fp2 = fopen(labelsPath, "rb");

    if (!fp || !fp2) {
        if (fp) fclose(fp);
        if (fp2) fclose(fp2);
        return false;
    }

    // Read bytes in flipped order
    int magicNumber = readFlippedInteger(fp);
    (void)magicNumber;
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
