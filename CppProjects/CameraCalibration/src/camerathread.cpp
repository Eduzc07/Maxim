#include "include/camerathread.h"

#include <opencv2/core/utility.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/calib3d.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/videoio.hpp>
#include <opencv2/highgui.hpp>

#include <QDebug>
#include <QFile>

using namespace cv;
using namespace std;

CameraThread::CameraThread()
{
    m_Setting = new Setting();

    //--- INITIALIZE VIDEOCAPTURE
    m_cap = new VideoCapture();
}

CameraThread::~CameraThread()
{
    delete m_cap;
}

void CameraThread::loadSetting(Setting* value)
{
   m_Setting = value;

   //Read Parameters
   readCalibParameters();
}

void CameraThread::openCamera()
{
    // open the default camera using default API
    // cap.open(0);
    // OR advance usage: select any API backend
    int deviceID = m_Setting->cameraID;             // 0 = open default camera
    int apiID = cv::CAP_ANY;      // 0 = autodetect default API
    // open selected camera using selected API
    m_cap->open(deviceID + apiID);
    // check if we succeeded
    if (!m_cap->isOpened()) {
        qDebug() << "ERROR! Unable to open camera\n";
        return;
    }

    int width = m_qvWidth[m_Setting->sizeImage];
    int height = m_qvHeight[m_Setting->sizeImage];
    //Set Size Image
    m_cap->set(cv::CAP_PROP_FRAME_WIDTH, width);
    m_cap->set(cv::CAP_PROP_FRAME_HEIGHT, height);
    m_cap->set(cv::CAP_PROP_BRIGHTNESS, 0);
    m_cap->set(cv::CAP_PROP_CONTRAST, 200);
    m_cap->set(cv::CAP_PROP_SATURATION, 200);
    m_cap->set(cv::CAP_PROP_EXPOSURE, 0);
}

void CameraThread::run()
{
    m_bRun = true;
    if (m_bCalib) {
        calibration();
        m_bCalib = false;
        m_bRun = false;
//        free(m_cap);
    }

    Mat frame;
    int n = 0;
    while(m_bRun){
        // wait for a new frame from camera and store it into 'frame'
        m_cap->read(frame);
        // check if we succeeded
        if (frame.empty()) {
            qDebug() << "ERROR! blank frame grabbed\n";
            break;
        }

        cv::Mat out = frame.clone();
        bool chess = false;
        if (!m_bUndistort)
            chess = checkChess(frame, out);

        if( m_Setting->flipVertical )
            flip( out, out, 0 );

        bool sleep = false;
        if(chess && m_bSave) {
            QString qs = QString("./images/test_Image_%1.jpg").
                    arg(QString::number(n + 1));
            qDebug()<< qs << "has been saved!";
            imwrite(qs.toUtf8().constData(), frame);
            n++;
            m_bSave = false;

//            bitwise_not(frame, out);
            bitwise_not(out, out);
            sleep = true;

            if (n >= (m_Setting->nrFrames)){
//                m_bRun = false;
                emit endLoop();
            }
        }

        if (m_bUndistort) {
            if (m_Setting->removeBorder){
                Mat dst;
                remap(frame, dst, m_map1, m_map2, INTER_LINEAR);
                out = dst(m_roi);
            } else {
                remap(frame, out, m_map1, m_map2, INTER_LINEAR, BORDER_CONSTANT, Scalar(100, 100, 100));
                rectangle(out, m_roi, Scalar(0, 255, 0), 2);
            }
        }

        // show live and wait for a key with timeout long enough to show images
        m_qImage = Mat2QImage(out);
        emit imageReady();

        //Display when is saving
        if(sleep)
            msleep(1000);
    }
}

QImage CameraThread::Mat2QImage(cv::Mat const& src)
{
    // 8-bits unsigned, NO. OF CHANNELS=1
    if(src.type() == CV_8UC1) {
        // Set the color table (used to translate colour indexes to qRgb values)
        QVector<QRgb> colorTable;
        for (int i=0; i<256; i++)
            colorTable.push_back(qRgb(i,i,i));

        // Copy input Mat
        const uchar *qImageBuffer = const_cast<const uchar*>(src.data);
        // Create QImage with same dimensions as input Mat
        QImage img( qImageBuffer,
                    src.cols,
                    src.rows,
                    static_cast<int>(src.step),
                    QImage::Format_Indexed8);

        img.setColorTable(colorTable);
        return img;
     }

     // 8-bits unsigned, NO. OF CHANNELS=3
     if(src.type() == CV_8UC3) {
         // Copy input Mat
         const uchar *qImageBuffer = const_cast<const uchar*>(src.data);
         // Create QImage with same dimensions as input Mat
         QImage img( qImageBuffer,
                     src.cols,
                     src.rows,
                     static_cast<int>(src.step),
                     QImage::Format_RGB888);

         return img.rgbSwapped();
     }
     return QImage();
}

bool CameraThread::checkChess(Mat const& src, cv::Mat& output)
{
    Mat view = src.clone();

    //! [find_pattern]
    vector<Point2f> pointBuf;

    bool found = false;
    int chessBoardFlags = CALIB_CB_ADAPTIVE_THRESH ;
    chessBoardFlags |= CALIB_CB_NORMALIZE_IMAGE;
    chessBoardFlags |= CALIB_CB_FAST_CHECK;

    found = findChessboardCorners( view, m_Setting->boardSize, pointBuf, chessBoardFlags);

    //! [pattern_found]
    if (found) {
        // improve the found corners' coordinate accuracy for chessboard

        Mat viewGray;
        cvtColor(view, viewGray, COLOR_BGR2GRAY);
        cornerSubPix( viewGray, pointBuf, Size(5, 5),
                      Size(-1,-1), TermCriteria( TermCriteria::EPS+TermCriteria::COUNT, 30, 0.0001 ));

        // Draw the corners.
        drawChessboardCorners( view, m_Setting->boardSize, Mat(pointBuf), found );
        view.copyTo(output);
    }
    //! [pattern_found]
    return found;
}


void CameraThread::calibration()
{
    vector<vector<Point2f>> imagePoints;
    Mat cameraMatrix, distCoeffs;
    Size imageSize;
    bool release_object = false;
    int winSize = 5;
    float grid_width = m_Setting->squareSize * (m_Setting->boardSize.width - 1);

    const Scalar RED(0,0,255), GREEN(0,255,0);
    int mode = CAPTURING;
    int numImages = 0;
    for(; ; numImages++){
        Mat view;
        QString qs = QString("./images/test_Image_%1.jpg").
                            arg(QString::number(numImages + 1));
        view = imread(qs.toUtf8().constData());

        //-----  If no more image, or got enough, then stop calibration and show result -------------
        if(mode == CAPTURING &&
                static_cast<int>(imagePoints.size()) >= m_Setting->nrFrames ) {
            if(runCalibrationAndSave(imageSize,
                                     cameraMatrix,
                                     distCoeffs,
                                     imagePoints,
                                     grid_width,
                                     release_object))
                mode = CALIBRATED;
            else
                mode = DETECTION;
        }

        // If there are no more images stop the loop
        if(view.empty()) {
            // if calibration threshold was not reached yet, calibrate now
            if( mode != CALIBRATED && !imagePoints.empty() )
                runCalibrationAndSave(imageSize,
                                      cameraMatrix,
                                      distCoeffs,
                                      imagePoints,
                                      grid_width,
                                      release_object);
            break;
        }

        imageSize = view.size();  // Format input image.
        if( m_Setting->flipVertical )
            flip( view, view, 0 );

        bool found;
        //! [find_pattern]
        vector<Point2f> pointBuf;

        int chessBoardFlags = CALIB_CB_ADAPTIVE_THRESH;
        chessBoardFlags |= CALIB_CB_NORMALIZE_IMAGE;

//        if(!m_Setting->useFisheye) {
            // fast check erroneously fails with high distortions like fisheye
            chessBoardFlags |= CALIB_CB_FAST_CHECK;
//        }

        // Find feature points on the input format
        switch( m_Setting->calibrationPattern ) {
        case Setting::CHESSBOARD:
            found = findChessboardCorners( view, m_Setting->boardSize, pointBuf, chessBoardFlags);
            break;
        case Setting::CIRCLES_GRID:
            found = findCirclesGrid( view, m_Setting->boardSize, pointBuf );
            break;
        case Setting::ASYMMETRIC_CIRCLES_GRID:
            found = findCirclesGrid( view, m_Setting->boardSize, pointBuf, CALIB_CB_ASYMMETRIC_GRID );
            break;
        default:
            found = false;
            break;
        }
        //! [find_pattern]

        //! [pattern_found]
        //! // If done with success,
        if (found) {
            // improve the found corners' coordinate accuracy for chessboard
            if( m_Setting->calibrationPattern == Setting::CHESSBOARD) {
                Mat viewGray;
                cvtColor(view, viewGray, COLOR_BGR2GRAY);
                cornerSubPix( viewGray,
                              pointBuf,
                              Size(winSize, winSize),
                              Size(-1,-1),
                              TermCriteria( TermCriteria::EPS+TermCriteria::COUNT, 30, 0.0001 ));
            }
            if( mode == CAPTURING)
                imagePoints.push_back(pointBuf);

            // Draw the corners.
            drawChessboardCorners( view, m_Setting->boardSize, Mat(pointBuf), found );
        }
        //! [pattern_found]
        //!  //----------------------------- Output Text ------------------------------------------------
        //! [output_text]
        string msg = (mode == CAPTURING) ? "100/100" :
                      mode == CALIBRATED ? "Calibrated" : "Press 'g' to start";
        int baseLine = 0;
        Size textSize = getTextSize(msg, 1, 1, 1, &baseLine);
        Point textOrigin(view.cols - 2*textSize.width - 10, view.rows - 2*baseLine - 10);

        if( mode == CAPTURING ) {
            msg = format( "[%d/%d]", static_cast<int>(imagePoints.size()),
                                    m_Setting->nrFrames);
        }
        putText( view, msg, textOrigin, 2, 1, mode == CALIBRATED ?  GREEN : RED);

        // show live and wait for a key with timeout long enough to show images
        m_qImage = Mat2QImage(view);
        msleep(500);
        emit imageReady();
    }

    // -----------------------Show the undistorted image for the image list ------------------------
    //! [show_results]
    Mat view, rview;

    m_map1.release();
    m_map2.release();

    if (m_Setting->useFisheye) {
        Mat newCamMat;
        fisheye::estimateNewCameraMatrixForUndistortRectify(cameraMatrix,
                                                            distCoeffs,
                                                            imageSize,
                                                            Matx33d::eye(),
                                                            newCamMat, 1);
        fisheye::initUndistortRectifyMap(cameraMatrix,
                                         distCoeffs,
                                         Matx33d::eye(),
                                         newCamMat,
                                         imageSize,
                                         CV_16SC2,
                                         m_map1,
                                         m_map2);
    } else {
        double alpha = m_Setting->removeBorder?0:1;
        initUndistortRectifyMap(cameraMatrix, distCoeffs, Mat(),
                                getOptimalNewCameraMatrix(cameraMatrix,
                                                          distCoeffs,
                                                          imageSize,
                                                          alpha,
                                                          imageSize,
                                                          &m_roi),
                                imageSize, CV_16SC2, m_map1, m_map2);
    }

    for(int i = 0; i < numImages; i++ ) {
        QString qs = QString("./images/test_Image_%1.jpg").
                arg(QString::number(i + 1));
        view = imread(qs.toUtf8().constData());
        if(view.empty())
            continue;

        if (m_Setting->removeBorder){
            remap(view, rview, m_map1, m_map2, INTER_LINEAR);
        } else {
            remap(view, rview, m_map1, m_map2, INTER_LINEAR, BORDER_CONSTANT, Scalar(100, 100, 100));
            rectangle(rview, m_roi, GREEN, 2);
        }

        m_qImage = Mat2QImage(rview);
        msleep(200);
        emit imageReady();
    }
    //! [show_results]
}

void CameraThread::readCalibParameters()
{
    m_dataCalibration = false;

    QFile file(m_Setting->outputFileName);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
        return;

    if (!file.exists()){
        return;
    }

    //! [file_read]
    Mat cameraMatrix, distCoeffs;
    int width, height;
    double avgError;
    const string inputSettingsFile = file.fileName().toUtf8().constData();
    FileStorage fs(inputSettingsFile, FileStorage::READ); // Read the settings
    if (!fs.isOpened()) {
        cout << "Could not open the configuration file: \"" << inputSettingsFile << "\"" << endl;
        return;
    }
    fs["image_width"] >> width;
    fs["image_height"] >> height;
    fs["camera_matrix"] >> cameraMatrix;
    fs["distortion_coefficients"] >> distCoeffs;
    fs["avg_reprojection_error"] >> avgError;
    fs.release();                                         // close Settings file
    //! [file_read]

    //Update Error
    emit updateError(avgError);
    Size imageSize = Size(width, height);

    if (m_Setting->useFisheye) {
        Mat newCamMat;
        fisheye::estimateNewCameraMatrixForUndistortRectify(cameraMatrix,
                                                            distCoeffs,
                                                            imageSize,
                                                            Matx33d::eye(),
                                                            newCamMat, 1);
        fisheye::initUndistortRectifyMap(cameraMatrix,
                                         distCoeffs,
                                         Matx33d::eye(),
                                         newCamMat,
                                         imageSize,
                                         CV_16SC2,
                                         m_map1,
                                         m_map2);
    } else {
//        double alpha = m_Setting->removeBorder?0:1;
        double alpha = 1.0;
        initUndistortRectifyMap(cameraMatrix, distCoeffs, Mat(),
                                getOptimalNewCameraMatrix(cameraMatrix,
                                                          distCoeffs,
                                                          imageSize,
                                                          alpha,
                                                          imageSize,
                                                          &m_roi),
                                imageSize, CV_16SC2, m_map1, m_map2);
    }
    m_dataCalibration = true;
}

//! [board_corners]
bool CameraThread::runCalibration(Size& imageSize,
                                  Mat& cameraMatrix,
                                  Mat& distCoeffs,
                                  vector<vector<Point2f> > imagePoints,
                                  vector<Mat>& rvecs,
                                  vector<Mat>& tvecs,
                                  vector<float>& reprojErrs,
                                  double& totalAvgErr,
                                  vector<Point3f>& newObjPoints,
                                  float grid_width,
                                  bool release_object)
{

    qDebug()<< "runCalibration";
    //! [fixed_aspect]
    cameraMatrix = Mat::eye(3, 3, CV_64F);
    cameraMatrix.at<double>(0,0) = 1.0;
    if(m_Setting->flag & CALIB_FIX_ASPECT_RATIO )
        cameraMatrix.at<double>(0,0) = m_Setting->aspectRatio;
    //! [fixed_aspect]
    if (m_Setting->useFisheye) {
        distCoeffs = Mat::zeros(4, 1, CV_64F);
    } else {
        distCoeffs = Mat::zeros(8, 1, CV_64F);
    }

    distCoeffs = Mat::zeros(8, 1, CV_64F);

    vector<vector<Point3f> > objectPoints(1);

    objectPoints[0].clear();
    calcBoardCornerPositions(objectPoints[0]);
    objectPoints[0][static_cast<unsigned int>(m_Setting->boardSize.width) - 1].x = objectPoints[0][0].x + grid_width;
    newObjPoints = objectPoints[0];

    objectPoints.resize(imagePoints.size(), objectPoints[0]);

    //Find intrinsic and extrinsic camera parameters
    double rms;

    if (m_Setting->useFisheye) {
        Mat _rvecs, _tvecs;
        rms = fisheye::calibrate(objectPoints,
                                 imagePoints,
                                 imageSize,
                                 cameraMatrix,
                                 distCoeffs,
                                 _rvecs,
                                 _tvecs,
                                 m_Setting->flag);

        rvecs.reserve(static_cast<unsigned int>(_rvecs.rows));
        tvecs.reserve(static_cast<unsigned int>(_tvecs.rows));
        for(int i = 0; i < int(objectPoints.size()); i++){
            rvecs.push_back(_rvecs.row(i));
            tvecs.push_back(_tvecs.row(i));
        }
    } else {
        int iFixedPoint = -1;
        if (release_object)
            iFixedPoint = m_Setting->boardSize.width - 1;
        rms = calibrateCameraRO(objectPoints,
                                imagePoints,
                                imageSize,
                                iFixedPoint,
                                cameraMatrix,
                                distCoeffs,
                                rvecs,
                                tvecs,
                                newObjPoints,
                                m_Setting->flag | CALIB_USE_LU);
    }

    if (release_object) {
        cout << "New board corners: " << endl;
        cout << newObjPoints[0] << endl;
        cout << newObjPoints[static_cast<unsigned int>(m_Setting->boardSize.width) - 1] << endl;
        cout << newObjPoints[static_cast<unsigned int>(m_Setting->boardSize.width) * (static_cast<unsigned int>(m_Setting->boardSize.height) - 1)] << endl;
        cout << newObjPoints.back() << endl;
    }

    qDebug() << "Re-projection error reported by calibrateCamera:"<< rms;

    bool ok = checkRange(cameraMatrix) && checkRange(distCoeffs);

    objectPoints.clear();
    objectPoints.resize(imagePoints.size(), newObjPoints);
    totalAvgErr = computeReprojectionErrors(objectPoints, imagePoints, rvecs, tvecs, cameraMatrix,
                                            distCoeffs, reprojErrs);

    return ok;
}


//! [board_corners]
void CameraThread::calcBoardCornerPositions(vector<Point3f>& corners)
{
    corners.clear();

    switch(m_Setting->calibrationPattern) {
    case Setting::CHESSBOARD:
    case Setting::CIRCLES_GRID:
        for( int i = 0; i < m_Setting->boardSize.height; ++i )
            for( int j = 0; j < m_Setting->boardSize.width; ++j )
                corners.push_back(Point3f(j*m_Setting->squareSize, i*m_Setting->squareSize, 0));
        break;

    case Setting::ASYMMETRIC_CIRCLES_GRID:
        for( int i = 0; i < m_Setting->boardSize.height; i++ )
            for( int j = 0; j < m_Setting->boardSize.width; j++ )
                corners.push_back(Point3f((2*j + i % 2)*m_Setting->squareSize, i*m_Setting->squareSize, 0));
        break;
    default:
        break;
    }
}
//! [board_corners]

//! [compute_errors]
double CameraThread::computeReprojectionErrors( const vector<vector<Point3f> >& objectPoints,
                                         const vector<vector<Point2f> >& imagePoints,
                                         const vector<Mat>& rvecs, const vector<Mat>& tvecs,
                                         const Mat& cameraMatrix , const Mat& distCoeffs,
                                         vector<float>& perViewErrors)
{
    vector<Point2f> imagePoints2;
    size_t totalPoints = 0;
    double totalErr = 0, err;
    perViewErrors.resize(objectPoints.size());

    for(size_t i = 0; i < objectPoints.size(); ++i ) {
        if (m_Setting->useFisheye) {
            fisheye::projectPoints(objectPoints[i], imagePoints2, rvecs[i], tvecs[i], cameraMatrix,
                                   distCoeffs);
        } else {
            projectPoints(objectPoints[i], rvecs[i], tvecs[i], cameraMatrix, distCoeffs, imagePoints2);
        }
        err = norm(imagePoints[i], imagePoints2, NORM_L2);

        size_t n = objectPoints[i].size();
        perViewErrors[i] = static_cast<float>(std::sqrt(err*err/n));
        totalErr        += err*err;
        totalPoints     += n;
    }

    return std::sqrt(totalErr/totalPoints);
}
//! [compute_errors]

//! [run_and_save]
bool CameraThread::runCalibrationAndSave(Size imageSize, Mat& cameraMatrix, Mat& distCoeffs,
                           vector<vector<Point2f> > imagePoints, float grid_width, bool release_object)
{
    vector<Mat> rvecs, tvecs;
    vector<float> reprojErrs;
    double totalAvgErr = 0;
    vector<Point3f> newObjPoints;

    bool ok = runCalibration(imageSize,
                             cameraMatrix,
                             distCoeffs,
                             imagePoints,
                             rvecs,
                             tvecs,
                             reprojErrs,
                             totalAvgErr,
                             newObjPoints,
                             grid_width,
                             release_object);

    qDebug() << (ok ? "Calibration succeeded." : "Calibration failed.")
         << " avg re projection error:" << totalAvgErr;

    emit updateError(totalAvgErr);

    if (ok)
        saveCameraParams(imageSize, cameraMatrix, distCoeffs, rvecs, tvecs,
                         reprojErrs, imagePoints, totalAvgErr, newObjPoints);

    m_dataCalibration = ok;
    qDebug()<< "saved!";
    return ok;
}

void CameraThread::saveCameraParams(Size &imageSize, Mat &cameraMatrix, Mat &distCoeffs, const vector<Mat> &rvecs, const vector<Mat> &tvecs, const vector<float> &reprojErrs, const vector<vector<Point2f> > &imagePoints, double totalAvgErr, const vector<Point3f> &newObjPoints)
{
    QString nameq = m_Setting->outputFileName;
    string name = nameq.toUtf8().constData();
    FileStorage fs( name, FileStorage::WRITE );

    time_t tm;
    time( &tm );
    struct tm *t2 = localtime( &tm );
    char buf[1024];
    strftime( buf, sizeof(buf), "%c", t2 );

    fs << "calibration_time" << buf;

    if( !rvecs.empty() || !reprojErrs.empty() )
        fs << "nr_of_frames" << static_cast<int>(std::max(rvecs.size(), reprojErrs.size()));
    fs << "image_width" << imageSize.width;
    fs << "image_height" << imageSize.height;
    fs << "board_width" << m_Setting->boardSize.width;
    fs << "board_height" << m_Setting->boardSize.height;
    fs << "square_size" << m_Setting->squareSize;

    if( m_Setting->flag & CALIB_FIX_ASPECT_RATIO )
        fs << "fix_aspect_ratio" << m_Setting->aspectRatio;

    if (m_Setting->flag) {
        std::stringstream flagsStringStream;
        if (m_Setting->useFisheye) {
            flagsStringStream << "flags:"
                              << (m_Setting->flag & fisheye::CALIB_FIX_SKEW ? " +fix_skew" : "")
                              << (m_Setting->flag & fisheye::CALIB_FIX_K1 ? " +fix_k1" : "")
                              << (m_Setting->flag & fisheye::CALIB_FIX_K2 ? " +fix_k2" : "")
                              << (m_Setting->flag & fisheye::CALIB_FIX_K3 ? " +fix_k3" : "")
                              << (m_Setting->flag & fisheye::CALIB_FIX_K4 ? " +fix_k4" : "")
                              << (m_Setting->flag & fisheye::CALIB_RECOMPUTE_EXTRINSIC ? " +recompute_extrinsic" : "");
        } else {
            flagsStringStream << "flags:"
                              << (m_Setting->flag & CALIB_USE_INTRINSIC_GUESS ? " +use_intrinsic_guess" : "")
                              << (m_Setting->flag & CALIB_FIX_ASPECT_RATIO ? " +fix_aspectRatio" : "")
                              << (m_Setting->flag & CALIB_FIX_PRINCIPAL_POINT ? " +fix_principal_point" : "")
                              << (m_Setting->flag & CALIB_ZERO_TANGENT_DIST ? " +zero_tangent_dist" : "")
                              << (m_Setting->flag & CALIB_FIX_K1 ? " +fix_k1" : "")
                              << (m_Setting->flag & CALIB_FIX_K2 ? " +fix_k2" : "")
                              << (m_Setting->flag & CALIB_FIX_K3 ? " +fix_k3" : "")
                              << (m_Setting->flag & CALIB_FIX_K4 ? " +fix_k4" : "")
                              << (m_Setting->flag & CALIB_FIX_K5 ? " +fix_k5" : "");
        }
        fs.writeComment(flagsStringStream.str());
    }

    fs << "flags" << m_Setting->flag;

    fs << "fisheye_model" << m_Setting->useFisheye;

    fs << "camera_matrix" << cameraMatrix;
    fs << "distortion_coefficients" << distCoeffs;

    fs << "avg_reprojection_error" << totalAvgErr;
    if (m_Setting->writeExtrinsics && !reprojErrs.empty())
        fs << "per_view_reprojection_errors" << Mat(reprojErrs);

    if(m_Setting->writeExtrinsics && !rvecs.empty() && !tvecs.empty() ) {
        CV_Assert(rvecs[0].type() == tvecs[0].type());
        Mat bigmat(static_cast<int>(rvecs.size()), 6, CV_MAKETYPE(rvecs[0].type(), 1));
        bool needReshapeR = rvecs[0].depth() != 1 ? true : false;
        bool needReshapeT = tvecs[0].depth() != 1 ? true : false;

        for( size_t i = 0; i < rvecs.size(); i++ ) {
            Mat r = bigmat(Range(int(i), int(i+1)), Range(0,3));
            Mat t = bigmat(Range(int(i), int(i+1)), Range(3,6));

            if(needReshapeR)
                rvecs[i].reshape(1, 1).copyTo(r);
            else {
                //*.t() is MatExpr (not Mat) so we can use assignment operator
                CV_Assert(rvecs[i].rows == 3 && rvecs[i].cols == 1);
                r = rvecs[i].t();
            }

            if(needReshapeT)
                tvecs[i].reshape(1, 1).copyTo(t);
            else {
                CV_Assert(tvecs[i].rows == 3 && tvecs[i].cols == 1);
                t = tvecs[i].t();
            }
        }
        fs.writeComment("a set of 6-tuples (rotation vector + translation vector) for each view");
        fs << "extrinsic_parameters" << bigmat;
    }

    if(m_Setting->writePoints && !imagePoints.empty() ) {
        Mat imagePtMat(static_cast<int>(imagePoints.size()),
                       static_cast<int>(imagePoints[0].size()),
                CV_32FC2);
        for( size_t i = 0; i < imagePoints.size(); i++ ) {
            Mat r = imagePtMat.row(int(i)).reshape(2, imagePtMat.cols);
            Mat imgpti(imagePoints[i]);
            imgpti.copyTo(r);
        }
        fs << "image_points" << imagePtMat;
    }

    if( m_Setting->writeGrid && !newObjPoints.empty() ) {
        fs << "grid_points" << newObjPoints;
    }
}//! [run_and_save]
