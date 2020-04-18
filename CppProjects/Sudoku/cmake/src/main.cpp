#include <iostream>
#include "include/sudoku.h"

int main(int argc, char *argv[])
{
    // if( argc != 2) {
    //   cout <<" Usage: display_image ImageToLoadAndDisplay" << endl;
    //   return -1;
    //  }

     Mat input;
     // sudoku = imread(argv[1], 0);   // Read the file
     input = imread("../install/images/image.jpg", IMREAD_COLOR);   // Read the file

     // Check for invalid input
     if(!input.data) {
         cout <<  "Could not open or find the image" << std::endl ;
         return -1;
     }

     Mat sudoku;
     cv::cvtColor(input, sudoku, COLOR_BGR2GRAY);

     Mat sudoku_original = sudoku.clone();
     Mat undistorted;
     Mat ohneGaus;

     int maxLength = getBox(input, undistorted, ohneGaus);


     // imshow("ohneGaus", ohneGaus);
     // waitKey(5);

     //maxLength,undistorted,ohneGaus
     //----------------------------------------------------------------

     Mat undistortedThreshed = undistorted.clone();
     adaptiveThreshold(undistorted, undistortedThreshed, 255, ADAPTIVE_THRESH_GAUSSIAN_C, THRESH_BINARY_INV, 21, 2);

     DigitRecognizer *dr = new DigitRecognizer();
     bool b = dr->train("../install/Character_Recognition/train-images.idx3-ubyte", "../install/Character_Recognition/train-labels.idx1-ubyte");

     int dist = ceil((double)maxLength/9);
     Mat currentCell = Mat(dist, dist, CV_8UC1);
     Mat numberCell = Mat(dist, dist, CV_8UC1);
     vector<vector<Point> > contours;

     imshow("Original", input);
     imwrite("../install/images/output.jpg", input);

     //Create a Mat
     Mat sudokuMat = cv::Mat::zeros(cv::Size(9, 9), CV_8UC1);

     for(int j = 0; j < 9; j++) {
        for(int i = 0; i < 9; i++){
            for(int y=0; y<dist && j*dist+y<undistortedThreshed.cols;y++){
                uchar* ptr = currentCell.ptr(y);
                for(int x=0;x<dist && i*dist+x<undistortedThreshed.rows;x++){
                    ptr[x] = undistortedThreshed.at<uchar>(j*dist+y, i*dist+x);
                }
             }

             for(int y=0;y<dist && j*dist+y<ohneGaus.cols;y++){
                uchar* ptr = numberCell.ptr(y);
                for(int x=0;x<dist && i*dist+x<ohneGaus.rows;x++){
                    ptr[x] = ohneGaus.at<uchar>(j*dist+y, i*dist+x);
                }
             }

             Moments m = cv::moments(currentCell, true);
             int area = m.m00;
//                 printf("Area: %d \n", area);
                // imshow("testing image", currentCell);
                // waitKey(0);

             if(area > 480){
//                  printf("Area: %d \n", area);
                Mat currentCell_o = currentCell.clone();
                findContours(currentCell_o, contours, RETR_LIST, CHAIN_APPROX_SIMPLE);

             for (size_t k = 0; k < contours.size(); k++) {
                vector < Point > cnt = contours[k];
//                    std::cout<<"Contour: "<<contourArea(cnt)<< std::endl;

                if (contourArea(cnt) > 50) {
                    Rect rec = boundingRect(cnt);
//                      std::cout<<"Heigh: "<<rec.height<< "> "<<currentCell.rows/3 <<std::endl;
//                      std::cout<<"width: "<< currentCell.cols*2/3<< "> "<<rec.width<< "> "<< currentCell.cols/5<<std::endl;
                    if (rec.height > currentCell.rows/3 && rec.width < currentCell.cols*2/3 && rec.width > currentCell.cols/5 ) {
                        Mat roi = numberCell(rec);

//                    sprintf(file,"%d.png", kik++);
//                    imwrite( file, roi );

                        Mat thresholdImage;
                        adaptiveThreshold(roi, thresholdImage, 255, 1, 1, 11, 2);

                        Mat outImage;
                        resize(thresholdImage, outImage, Size(sizex, sizey));

                        imshow("roi", outImage);
                        waitKey(40);

                        int number = dr->classify( outImage);
                        // printf("%d", number);
                        sudokuMat.at<uchar>(j, i) = number;
                        break;
                     }
                 }
              }
          }
          // else {
          //   printf("-");
          // }
       }
       // printf("\n");
    }

    cout <<  sudokuMat << endl;

    //------------------------------------------------------------------_

    destroyWindow("roi");
    waitKey(0);                                          // Wait for a keystroke in the window

    destroyAllWindows();
    return 0;
}
