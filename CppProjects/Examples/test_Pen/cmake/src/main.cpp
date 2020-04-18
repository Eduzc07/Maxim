/**
 * @file introduction_to_pca.cpp
 * @brief This program demonstrates how to use OpenCV PCA to extract the orienation of an object
 * @author OpenCV team
 */
#include <opencv2/opencv.hpp>

#include <cstdio>
#include <iostream>

using namespace std;
using namespace cv;

Rect getSize(Mat&);
double getWear(Rect, Rect);

/**
 * @function main
 */
int main(int, char** argv)
{
//! [pre-process]
    // Load image
    Mat refPen = imread("../cmake/images/pen1.jpg");

    // Check if image is loaded successfully
    if(!refPen.data || refPen.empty()) {
        cout << "Problem loading image!!!" << endl;
        return 0;
    }

    Mat testPen = imread("../cmake/images/pen2.jpg");
    //Mat testPen = imread("images/1550270138870.jpg");
    //Mat testPen = imread("images/1550270132516.jpg");
    //Mat testPen = imread("images/1550270128376.jpg");
    //Mat testPen = imread("images/1550270125003.jpg");
    //Mat testPen = imread("images/1550270121324.jpg");
    //Mat testPen = imread("images/1550270108193.jpg");
    //Mat testPen = imread("images/1550270106161.jpg");
    //Mat testPen = imread("images/1550270098714.jpg");
    //Mat testPen = imread("images/1550270090026.jpg");
    //Mat testPen = imread("images/1550269817815.jpg");
    //Mat testPen = imread("images/1550269802706.jpg");

    // Check if image is loaded successfully
    if(!testPen.data || testPen.empty()) {
        cout << "Problem loading image!!!" << endl;
        return 0;
    }

    Rect refRect = getSize(refPen);
    cout << refRect.width << "---"<< refRect.height<< endl;

    Rect testRect = getSize(testPen);
    cout << testRect.width << "---"<< testRect.height<< endl;

    double wear = getWear(refRect, testRect);
    cout << wear  << endl;

    char name[10];
    sprintf (name, "%0.1f%%", wear);

    cv::putText(testPen,
                name,
                cv::Point(testPen.cols / 2 - 30, testPen.rows / 2 - 250), // Coordinates
                cv::FONT_HERSHEY_DUPLEX, // Font
                0.8, // Scale. 2.0 = 2x bigger
                cv::Scalar(255,255,255), // BGR Color
                1); // Line Thickness (Optional)

    //Display
    imshow("testPen", testPen);
    waitKey(0);
    return 0;
}

Rect getSize(Mat& srcOrig){

    //Pre ROI
    Rect newBox = Rect(srcOrig.cols / 2 - 60, srcOrig.rows / 2 - 280, 120, 560);
    Mat image_roi = srcOrig.clone()(newBox);

    Mat src;
    image_roi.copyTo(src);

    //rectangle( src, newBox.tl(), newBox.br(), color, 2 );

    // Create a kernel that we will use to sharpen our image
    Mat kernel = (Mat_<float>(3,3) <<
                      1,  1, 1,
                      1, -8, 1,
                      1,  1, 1);
    // an approximation of second derivative, a quite strong kernel
    // do the laplacian filtering as it is
    // well, we need to convert everything in something more deeper then CV_8U
    // because the kernel has some negative values,
    // and we can expect in general to have a Laplacian image with negative values
    // BUT a 8bits unsigned int (the one we are working with) can contain values from 0 to 255
    // so the possible negative number will be truncated
    Mat imgLaplacian;
    filter2D(src, imgLaplacian, CV_32F, kernel);
    Mat sharp;
    src.convertTo(sharp, CV_32F);
    Mat imgResult = sharp - imgLaplacian;
    // convert back to 8bits gray scale
    imgResult.convertTo(imgResult, CV_8UC3);
    imgLaplacian.convertTo(imgLaplacian, CV_8UC3);
    //imshow( "Laplace Filtered Image", imgLaplacian );
    //imshow( "New Sharped Image", imgResult );

    // Create binary image from source image
    Mat bw;
    cvtColor(imgResult, bw, COLOR_BGR2GRAY);
    threshold(bw, bw, 40, 255, THRESH_BINARY_INV | THRESH_OTSU);
    //imshow("Binary Image", bw);

    // Perform the distance transform algorithm
    Mat dist;
    distanceTransform(bw, dist, DIST_L2, 3);
    // Normalize the distance image for range = {0.0, 1.0}
    // so we can visualize and threshold it
    normalize(dist, dist, 0, 1.0, NORM_MINMAX);
    //imshow("Distance Transform Image", dist);

    // Threshold to obtain the peaks
    // This will be the markers for the foreground objects
    //threshold(dist, dist, 0.05, 1.0, THRESH_BINARY);

    // Dilate a bit the dist image
    Mat kernel1 = Mat::ones(3, 3, CV_8U);
    dilate(bw, bw, kernel1);

    Mat element = getStructuringElement(MORPH_RECT, Size(5, 5));
    /// Apply the specified morphology operation
    morphologyEx( bw, bw, MORPH_OPEN, element );
    imshow("Peaks", bw);

    // Create the CV_8U version of the distance image
    // It is needed for findContours()
    Mat dist_8u;
    //dist.convertTo(dist_8u, CV_8U);
    bw.copyTo(dist_8u);
    // Find total markers
    vector<vector<Point> > contours;

    findContours(dist_8u, contours, RETR_EXTERNAL, CHAIN_APPROX_SIMPLE);
    // Create the marker image for the watershed algorithm
    Mat markers = Mat::zeros(dist.size(), CV_32S);
    // Draw the foreground markers
    for (size_t i = 0; i < contours.size(); i++) {
        drawContours(markers, contours, static_cast<int>(i), Scalar(static_cast<int>(i)+1), -1);
    }

    // Draw the background marker
    Rect boxWater = Rect(20, 20, 80, 520);
    rectangle( markers, boxWater.tl(), boxWater.br(), Scalar(255), 3 );

    Mat newMat;
    markers.convertTo(newMat, CV_8U);
    //imshow("Markers", newMat*200);

    // Perform the watershed algorithm
    watershed(imgResult, markers);
    Mat mark;
    markers.convertTo(mark, CV_8U);
    bitwise_not(mark, mark);
    //imshow("Markers_v2", mark); // uncomment this if you want to see how the mark
    // image looks like at that point
    // Generate random colors
    vector<Vec3b> colors;
    for (size_t i = 0; i < contours.size(); i++) {
        int b = theRNG().uniform(0, 256);
        int g = theRNG().uniform(0, 256);
        int r = theRNG().uniform(0, 256);
        colors.push_back(Vec3b((uchar)b, (uchar)g, (uchar)r));
    }

    // Create the result image
    Mat dst = Mat::zeros(markers.size(), CV_8UC3);
    // Fill labeled objects with random colors
    for (int i = 0; i < markers.rows; i++) {
        for (int j = 0; j < markers.cols; j++) {
            int index = markers.at<int>(i,j);
            if (index > 0 && index <= static_cast<int>(contours.size())) {
                dst.at<Vec3b>(i,j) = colors[index-1];
            }
        }
    }

    contours.clear();
    findContours(dist_8u, contours,  RETR_TREE, CHAIN_APPROX_SIMPLE);
    vector<vector<Point> > contours_poly( contours.size() );
    vector<Rect> boundRect( contours.size() );

    //Enclose Rectangle
    for( size_t i = 0; i < contours.size(); i++ ) {
        approxPolyDP( contours[i], contours_poly[i], 3, true );
        boundRect[i] = boundingRect( contours_poly[i] );
    }

    Mat drawing = Mat::zeros( imgResult.size(), CV_8UC3 );

    for( size_t i = 0; i< contours.size(); i++ ) {
        Scalar colorRect = Scalar( theRNG().uniform(0, 256), theRNG().uniform(0,256), theRNG().uniform(0,256) );
        rectangle( drawing, boundRect[i].tl(), boundRect[i].br(), colorRect, 2 );
    }

    //imshow( "Contours", drawing );

    Mat destMat = Mat::zeros( srcOrig.size(), CV_8UC3 );
    dst.copyTo( destMat(newBox) );
    //imshow("destMat", destMat);

    dst = dst*0.4 + imgResult*0.6;

    rectangle( dst, boundRect[0].tl(), boundRect[0].br(), Scalar(0, 200,0), 2 );
    // Visualize the final image
    //imshow("Final Result", dst);
    Point init = Point(newBox.x, newBox.y);

    srcOrig = destMat*0.4 + srcOrig*1.0;
    rectangle(srcOrig, init + boundRect[0].tl(), init + boundRect[0].br(), Scalar(0, 200,0), 2 );

    //imshow("image_roi", image_roi);
    return boundRect[0];
}


double getWear(Rect refSize, Rect testSize)
{
    double widthRef;
    double heightRef;

    if (refSize.height > refSize.width){
        widthRef = refSize.width;
        heightRef = refSize.height;
    } else {
        widthRef = refSize.height;
        heightRef = refSize.width;
    }

    double widthTest;
    double heightTest;

    if (testSize.height > testSize.width){
        widthTest = testSize.width;
        heightTest = testSize.height;
    } else {
        widthTest = testSize.height;
        heightTest = testSize.width;
    }

    double f = widthRef / widthTest;

    double wear = f*heightTest*100./heightRef;
    return wear;
}
