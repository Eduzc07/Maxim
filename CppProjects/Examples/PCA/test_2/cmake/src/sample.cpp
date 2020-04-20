#include "include/sample.h"

vector<Mat> MatData; ///Save all the images in one vector
vector<int> valnm;   ///Save the value in nm of each image

const char* window_name = "Edge Map";
const char* original_window = "Original";

void PCA_image(vector<String> filenames)
{
    namedWindow("PCA", WINDOW_AUTOSIZE );
    namedWindow(original_window, WINDOW_AUTOSIZE );

    int num_image00=0;
    int num_image01=1;

    int images=0;
    int pca_num = filenames.size();


    /// Create a Trackbar for user to enter threshold
    createTrackbar( "Image:", original_window, &images, pca_num-1 );
    createTrackbar( "PCA:", original_window, &pca_num, pca_num-1);

    createTrackbar( "Image 1:","PCA", &num_image00, pca_num-1 );
    createTrackbar( "Image 2:","PCA", &num_image01, pca_num-1);

    //! [pca]
    //Construct a buffer used by the pca analysis
    int sz = MatData[0].cols*MatData[0].rows;
    Mat data_pca = Mat::zeros(sz,MatData.size(), CV_64FC1);

    Mat m = data_pca.clone();
    createData(MatData,data_pca,m);

//    //Perform PCA analysis
//    PCA pca_analysis(data_pca, Mat(), CV_PCA_DATA_AS_ROW);

//    //Store the eigenvalues and eigenvectors
//    Mat vectors = pca_analysis.eigenvectors;
//    Mat values = pca_analysis.eigenvalues;
    //![pca]

//    cout<<values<<endl;
//    cout<<vectors<<endl;

    while(1){
        /// get a Trackbar for user
        images = getTrackbarPos("Image:", original_window);
        pca_num = getTrackbarPos("PCA:", original_window);

        PCA pca_analysis(data_pca, Mat(), PCA::DATA_AS_ROW, pca_num);
        Mat points = pca_analysis.project(data_pca); // project into the eigenspace, thus the image becomes a "point"
        Mat original = pca_analysis.backProject(points); // re-create the image from the "point"

        Mat B = getMat(127 + points.col(images), MatData[images].size(),1);
        imshow(original_window, B);
	
	//Plot
        /// get a Trackbar for user
        num_image00 = getTrackbarPos("Image 1:","PCA");
        num_image01 = getTrackbarPos("Image 2:","PCA");

        plotPCA(filenames, num_image00, num_image01);

        /// Wait until user exit program by pressing a key
        if (waitKey(30) >= 0)
            break;
    }
    destroyAllWindows();
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
    }
}

float meanMat(Mat src){
    Scalar tempVal = mean(src);
    return tempVal.val[0];
}

Mat getMat(Mat dataMat, Size sizeData, double kRescaleFactor=1){

    //array to Mat
    dataMat.convertTo(dataMat, CV_8UC1);
    Mat A = Mat(sizeData, CV_8UC1, dataMat.data, Mat::AUTO_STEP);

    Mat newSize;
    resize(A, newSize, Size(0, 0), kRescaleFactor, kRescaleFactor);

    return newSize;
}

void createData(vector<Mat> Matdata,Mat &data,Mat &m){

    //All the iamges should be the same size
    int sz = data.rows;

    for (int i=0;i<Matdata.size();i++) {

        std::vector<double> array;
        if (Matdata[i].isContinuous()) {
          array.assign(Matdata[i].datastart,MatData[i].dataend);
        }

        Mat newMat = Mat(array).reshape(0,sz);
        newMat.col(0).copyTo(data.col(i));


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
        }else{
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

    for (int i=0; i<src0.rows*src0.cols; i++){
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
    drawAxis(axis, cntr, p1, Scalar(0, 0, 255), 1);
    drawAxis(axis, cntr, p2, Scalar(255, 0, 0), 1);
//! [visualization]

    imshow("PCA",axis);
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

