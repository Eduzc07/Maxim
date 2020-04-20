#include "include/sample.h"

/// Global variables
//Mat src,src_gray;
//Mat detected_edges;
//Mat new_mat,dst;

vector<Mat> MatData; ///Save all the images in one vector
vector<int> valnm;   ///Save the value in nm of each image

const char* window_name = "Edge Map";
const char* original_window = "Original";

void PCA_image(String file)
{
    Mat axis = Mat::zeros(510, 510, CV_8UC3);
    Mat axis0 = Mat::zeros(700, 700, CV_8UC3);
    Mat axis1 = Mat::zeros(510, 900, CV_8UC3);

    vector<String> fileNames;
    read_files(fileNames, file); ///Read the images into the current file

    //............................................................
//    int sz = 10;
//    Mat x = (Mat_<uchar>(10,1) << 25,5,22,19,31,23,20,10,15,11);
//    Mat y = (Mat_<uchar>(10,1) << 24,7,29,22,30,27,16,11,16,9);

//    Mat xn = Mat::zeros(sz,1, CV_64FC1);
//    Mat yn = Mat::zeros(sz,1, CV_64FC1);

//    for (int i=0;i<sz;i++){

//        double x1=8*x.data[i];
//        double x2=8*y.data[i];

//        Point2f Pa=Point2f(x1*2,510-x2*2);
//        circle(axis, Pa, 8, Scalar(255,10+20*i,0), -1 );

////        xn.at<double>(i,0)=Pa.x;
////        yn.at<double>(i,0)=Pa.y;
//        xn.at<double>(i,0)=x.data[i]/10.0;
//        yn.at<double>(i,0)=y.data[i]/10.0;

//    }

//    MatData.clear();
//    MatData.push_back(xn);
//    MatData.push_back(yn);

     //............................................................
    int sz = MatData[0].cols*MatData[0].rows;
//    Mat xn = Mat::zeros(sz,1, CV_64FC1);
//    Mat yn = Mat::zeros(sz,1, CV_64FC1);

    for (int i=0;i<sz;i++){
        double x1=MatData[0].data[i];
        double x2=MatData[1].data[i];

        Point2f Pa=Point2f(x1,x2);

        double color=i*255/(sz);
        circle(axis, Pa, 2, Scalar(0,color,0), -1 );
//        xn.at<double>(i,0)=Pa.x;
//        yn.at<double>(i,0)=Pa.y;
    }

//    MatData.clear();
//    MatData.push_back(xn);
//    MatData.push_back(yn);
    //............................................................

    //! [pca]
    Mat data_pca = Mat::zeros(sz,MatData.size(), CV_64FC1);
    Mat m = data_pca.clone();
    createData(MatData,data_pca, m);

    //------------------------------------------------
    for (int i=0;i<sz;i++){
        double x1=m.at<double>(i,0);
        double x2=m.at<double>(i,1);

        Point2f Pa=Point2f(350+x1,350+x2);
        double color=i*255/(sz);
        circle(axis0, Pa, 2, Scalar(0,color,0), -1 );
    }

    line(axis0, Point(350, 0), Point(350,700), Scalar(255,255,255), 1, LINE_AA);
    line(axis0, Point(0, 350), Point(700,350), Scalar(255,255,255), 1, LINE_AA);
    //------------------------------------------------

    //Perform PCA analysis
    PCA pca_analysis(data_pca, Mat(), PCA::DATA_AS_ROW,2);

    //Store the eigenvalues and eigenvectors
    Mat vectors = pca_analysis.eigenvectors;
    Mat values = pca_analysis.eigenvalues;

//    cout << "vectors" <<endl;
//    cout << vectors <<endl;
//    cout << values <<endl;

    Mat points = pca_analysis.project(data_pca); // project into the eigenspace, thus the image becomes a "point"
    Mat reconstruction = pca_analysis.backProject(points); // re-create the image from the "point"

//    cout << "Originals" <<endl;
//    cout << reconstruction.col(0) <<endl;

    //![pca]

    for (int i=0; i<sz; i++){
        double x1=points.at<double>(i,0);
        double x2=points.at<double>(i,1);

        Point2f Pa=Point2f(450.0+x1,255.0+x2);

        Point2f a(0,0);
        Point2f b(450,255);
        double res = cv::norm(cv::Mat(a),cv::Mat(b));
        double pos = cv::norm(cv::Mat(Pa),cv::Mat(b));

        double val;
        val=pos*255.0/120;
        double val2=i*255/sz;
        circle(axis1, Pa, 2, Scalar(val2,val2,255-val), -1 );
    }

    line(axis1, Point(450,0), Point(450,900), Scalar(255,255,255), 1, LINE_AA);
    line(axis1, Point(0,255), Point(900,255), Scalar(255,255,255), 1, LINE_AA);

    //--------------------------------------------------------------------
    int images = 0;
    Mat B = getMat(reconstruction.col(1),MatData[images].size(),1.0);
    imshow(original_window,B);
    //--------------------------------------------------------------------

    PCA pca_plot(data_pca, Mat(), PCA::DATA_AS_ROW);

    //Store the center of the object
    Point cntr = Point(static_cast<int>(pca_plot.mean.at<double>(0, 0)),
                      static_cast<int>(pca_plot.mean.at<double>(0, 1)));

    //Store the eigenvalues and eigenvectors
    vector<Point2d> eigen_vecs(2);
    vector<double> eigen_val(2);
    for (int i = 0; i < 2; ++i) {
        eigen_vecs[i] = Point2d(pca_plot.eigenvectors.at<double>(i, 0),
                                pca_plot.eigenvectors.at<double>(i, 1));

        eigen_val[i] = pca_plot.eigenvalues.at<double>(0, i);
    }

//! [pca]
//! [visualization]
    // Draw the principal components
    circle(axis, cntr, 10, Scalar(255, 0, 255), -1);
    Point p1 = cntr + 0.02 * Point(static_cast<int>(eigen_vecs[0].x * eigen_val[0]), static_cast<int>(eigen_vecs[0].y * eigen_val[0]));
    Point p2 = cntr - 0.02 * Point(static_cast<int>(eigen_vecs[1].x * eigen_val[1]), static_cast<int>(eigen_vecs[1].y * eigen_val[1]));
    drawAxis(axis, cntr, p1, Scalar(0, 255, 0), 0.5); //PCA
    drawAxis(axis, cntr, p2, Scalar(255, 0, 0), 3);
//! [visualization]

    imshow("Compute PCA",axis);
    imshow("x0",axis0);
    imshow("x1",axis1);
    waitKey();

    //............................................................

//    namedWindow("PCA", CV_WINDOW_AUTOSIZE );
//    namedWindow(original_window, CV_WINDOW_AUTOSIZE );

//    int num_image00=0;
//    int num_image01=1;

//    int images=0;
//    int pca_num=0;
//    int pca_ini=0;

//    /// Create a Trackbar for user to enter threshold
//    createTrackbar( "Image:",original_window, &images, glob_result.gl_pathc-1 );
//    createTrackbar( "PCA:",original_window, &pca_num, glob_result.gl_pathc-1);
//    createTrackbar( "PCAi:",original_window, &pca_ini, glob_result.gl_pathc-1);

//    createTrackbar( "Image 1:","PCA", &num_image00,glob_result.gl_pathc-1 );
//    createTrackbar( "Image 2:","PCA", &num_image01, glob_result.gl_pathc-1);

//    //! [pca]
//    //Construct a buffer used by the pca analysis
//    int sz = MatData[0].cols*MatData[0].rows;
//    Mat data_pca = Mat::zeros(sz,MatData.size(), CV_64FC1);

//    Mat m = data_pca.clone();
//    createData(MatData,data_pca,m);

//    //Perform PCA analysis
//    PCA pca_analysis(data_pca, Mat(), CV_PCA_DATA_AS_ROW);

//    //Store the eigenvalues and eigenvectors
//    Mat vectors = pca_analysis.eigenvectors;
//    Mat values = pca_analysis.eigenvalues;
//    //![pca]

//    cout<<values<<endl;
//    cout<<vectors<<endl;

//    while(1){

//    /// get a Trackbar for user to enter threshold
//        images=getTrackbarPos("Image:",original_window);
//        pca_num=getTrackbarPos("PCA:",original_window);
//        pca_ini=getTrackbarPos("PCAi:",original_window);


//        Mat original;
//        getOriginal(vectors,m,original,pca_num+1,pca_ini);

//        Mat B=getMat(original.row(images)+meanMat(MatData[images]),MatData[images].size(),0.6);
//        imshow(original_window,B);

//Plot
//        /// get a Trackbar for user to enter threshold
//        num_image00=getTrackbarPos("Image 1:","PCA");
//        num_image01=getTrackbarPos("Image 2:","PCA");

//        plotPCA(glob_result,num_image00,num_image01);

//        /// Wait until user exit program by pressing a key
//        if (waitKey(30) >= 0)
//            break;
//    }
}

/**
 * @brief read_files
 * @param glob_result
 */
void read_files(vector<String> &filenames, String folder)
{
    String pattern = "*.bmp";
    utils::fs::glob(folder, pattern, filenames);

    Mat src;
    for(size_t i=0; i<filenames.size();++i) {
        src = imread(filenames[i], IMREAD_GRAYSCALE);
	MatData.push_back(src);

        string str = String(folder); // from cv::String to std::String
        size_t found = str.find("f");
        double posFound = static_cast<double>(found);
        string nmS = str.substr(posFound+1, 2);
        int nm = atoi(nmS.c_str());
        valnm.push_back(400 + 50*(nm-1)); // Die Wellenläge fängt in 400 an.
    }
}

float meanMat(Mat src){
    Scalar tempVal = mean(src);
    return tempVal.val[0];
}

Mat getMat(Mat dataMat,Size sizeData, double kRescaleFactor = 1){

    //array to Mat
    dataMat.convertTo(dataMat, CV_8UC1);
    Mat A = Mat(sizeData, CV_8UC1, dataMat.data, Mat::AUTO_STEP);

    Mat newSize;
    resize(A, newSize, Size(0, 0), kRescaleFactor, kRescaleFactor);

    return newSize;
}

void createData(vector<Mat> Matdata,Mat &data,Mat &m)
{
    //All the iamges should be the same size
    int sz = data.rows;
    Mat newMat;
    std::vector<double> array;

    for (int i=0;i<Matdata.size();i++) {

        switch(Matdata[i].type()) {
        case CV_8UC1:
            if (Matdata[i].isContinuous()) {
                array.assign(Matdata[i].datastart,MatData[i].dataend);
            }
            newMat = Mat(array).reshape(0,sz);
            newMat.col(0).copyTo(data.col(i));
            break;

        case CV_64FC1:
            Matdata[i].col(0).copyTo(data.col(i));
            newMat = Matdata[i].col(0);
            break;
        }

        Mat mi=newMat-meanMat(Matdata[i]);
        mi.col(0).copyTo(m.col(i));
    }
}



void getOriginal(Mat vectors,Mat m,Mat &original,int PCAn,int PCAinit=0)
{
    Mat final=m.clone();

    for (int i=0;i<vectors.cols;i++) {
        Mat vectorn;
        if (PCAinit<=i && i<PCAn){
            vectorn=vectors.col(i);
        } else {
            vectorn=Mat::zeros(vectors.cols,1,vectors.type());
        }

        Mat finaln=vectorn.t()*m.t();
        finaln=finaln.t();
        finaln.col(0).copyTo(final.col(i));

    }

    original=vectors*final.t();
}

void plotPCA(vector<String> filenames, int num_image00, int num_image01)
{
    /// Load an image
    int image = num_image00;
    int image1 = num_image01;
    Mat src0 = imread(filenames[image], IMREAD_GRAYSCALE);
    if( !src0.data )
      { return; }

    Mat src1 = imread(filenames[image1], IMREAD_GRAYSCALE);
    if( !src1.data )
      { return; }

    int sz = src0.cols*src0.rows;
    Mat data_pts = Mat::zeros(sz,MatData.size(), CV_64FC1);
    Mat axis = Mat::zeros(510,510,CV_8UC3);

    for (int i=0;i<src0.rows*src0.cols;i++){
            double x1=src0.data[i];
            double x2=src1.data[i];
            Point2f Pa=Point2f(x1*2,510-x2*2);
            double color=i*255/(src0.cols*src0.rows);
            circle(axis, Pa, 2, Scalar(0,color,0), -1 );
            //
            data_pts.at<double>(i, 0) = Pa.x;
            data_pts.at<double>(i, 1) = Pa.y;
    }

    //Perform PCA analysis
    PCA pca_plot(data_pts, Mat(), PCA::DATA_AS_ROW);

    //Store the center of the object
    Point cntr = Point(static_cast<int>(pca_plot.mean.at<double>(0, 0)),
                      static_cast<int>(pca_plot.mean.at<double>(0, 1)));

    //Store the eigenvalues and eigenvectors
    vector<Point2d> eigen_vecs(2);
    vector<double> eigen_val(2);
    for (int i = 0; i < 2; ++i) {
        eigen_vecs[i] = Point2d(pca_plot.eigenvectors.at<double>(i, 0),
                                pca_plot.eigenvectors.at<double>(i, 1));

        eigen_val[i] = pca_plot.eigenvalues.at<double>(0, i);
    }

//! [pca]
//! [visualization]
    // Draw the principal components
    circle(axis, cntr, 10, Scalar(255, 0, 255), -1);
    Point p1 = cntr + 0.02 * Point(static_cast<int>(eigen_vecs[0].x * eigen_val[0]), static_cast<int>(eigen_vecs[0].y * eigen_val[0]));
    Point p2 = cntr - 0.02 * Point(static_cast<int>(eigen_vecs[1].x * eigen_val[1]), static_cast<int>(eigen_vecs[1].y * eigen_val[1]));
    drawAxis(axis, cntr, p1, Scalar(0, 0, 255), 0.2);
    drawAxis(axis, cntr, p2, Scalar(255, 0, 0), 5);
//! [visualization]

    imshow("PCA", axis);
}

/**
 * @function drawAxis
 */
void drawAxis(Mat& img, Point p, Point q, Scalar colour, const float scale = 0.2)
{
//! [visualization1]
    double angle;
    double hypotenuse;
    angle = atan2( (double) p.y - q.y, (double) p.x - q.x ); // angle in radians
    hypotenuse = sqrt( (double) (p.y - q.y) * (p.y - q.y) + (p.x - q.x) * (p.x - q.x));
//    double degrees = angle * 180 / CV_PI; // convert radians to degrees (0-180 range)
//    cout << "Degrees: " << abs(degrees - 180) << endl; // angle in 0-360 degrees range

    // Here we lengthen the arrow by a factor of scale
    q.x = (int) (p.x - scale * hypotenuse * cos(angle));
    q.y = (int) (p.y - scale * hypotenuse * sin(angle));
    line(img, p, q, colour, 2, LINE_AA);

    // create the arrow hooks
    p.x = (int) (q.x + 9 * cos(angle + CV_PI / 4));
    p.y = (int) (q.y + 9 * sin(angle + CV_PI / 4));
    line(img, p, q, colour, 2, LINE_AA);

    p.x = (int) (q.x + 9 * cos(angle - CV_PI / 4));
    p.y = (int) (q.y + 9 * sin(angle - CV_PI / 4));
    line(img, p, q, colour, 2, LINE_AA);
//! [visualization1]
}

//--------------------------------------------------------------------------------------------------------------------
//Color Wavelenght
//source: http://www.efg2.com/Lab/ScienceAndEngineering/Spectra.htm
//--------------------------------------------------------------------------------------------------------------------
void newColor(double wv, Scalar &Val)
{
  double R,G,B;
  R = 0;
  G = 0;
  B = 0;
  double Gamma=0.80;
  int IntensityMax = 255;
  double factor=0;

  if(wv>=380 && wv<=419){
     factor = 0.3 + 0.7*(wv - 380) / (420 - 380);
  }

  if(wv>419 && wv<=700){
     factor = 1.0;
  }

  if(wv>700 && wv<=850){
    factor = 0.3 + 0.7*(780-wv)/(780-700);
  }


  if(wv>380 && wv<=439){
     R= -(wv - 440) / (440 - 380);
     G= 0.0;
     B= 1.0;
  }

  if(wv>439 && wv<=489){
      R= 0.0;
      G=(wv - 440) / (490 - 440);;
      B= 1.0;
  }

  if(wv>489 && wv<=509){
      R= 0.0;
      G=1.0;
      B= -(wv - 510) / (510 - 490);
  }

  if(wv>509 && wv<=579){
      R=(wv - 510) / (580 - 510);
      G=1.0;
      B=0.0;
  }

  if(wv>579 && wv<=644){
     R= 1.0;
     G= -(wv - 645) / (645 - 580);
     B= 0.0;
  }

  if(wv>644 && wv<=850){
     R= 1.0;
     G= 0.0;
     B= 0.0;
  }

  if (R==0){
     R=0.0;
  }else{
    R= IntensityMax*pow(R*factor,Gamma);
  }

  if (G==0){
    G=0.0;
  }
  else{
    G= IntensityMax*pow(G*factor,Gamma);
  }

  if (B==0){
    B=0.0;
  }
  else{
    B= IntensityMax*pow(B*factor,Gamma);
  }
  Val=Scalar(B,G,R);
}
