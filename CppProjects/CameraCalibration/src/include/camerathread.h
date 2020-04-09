#ifndef CAMERATHREAD_H
#define CAMERATHREAD_H

#include <opencv2/core.hpp>
#include <iostream>
#include <stdio.h>
#include "Setting.h"

#include <QThread>
#include <QImage>
#include <QVector>

class CameraThread : public QThread
{
    Q_OBJECT
public:
    CameraThread();
    ~CameraThread();
    enum { DETECTION = 0, CAPTURING = 1, CALIBRATED = 2 };

    void stop(){m_bRun = false;}
    QImage getImage(){ return m_qImage;}
    void save(){ m_bSave = true;}

    void startCalibration(){ m_bCalib = true;}
    void loadSetting(Setting* value);
    bool isCalibrate(){ return m_dataCalibration;}
    void openCamera();

signals:
    void imageReady();
    void endLoop();
    void updateError(double);

public slots:
    void setUndistort(bool value){ m_bUndistort = value;}

protected:
    void run();

private:
    QImage Mat2QImage(cv::Mat const& src);
    bool checkChess(cv::Mat const& src, cv::Mat& output);

    void calcBoardCornerPositions(std::vector<cv::Point3f>& corners);

    double computeReprojectionErrors( const std::vector<std::vector<cv::Point3f> >& objectPoints,
                                      const std::vector<std::vector<cv::Point2f> >& imagePoints,
                                      const std::vector<cv::Mat>& rvecs,
                                      const std::vector<cv::Mat>& tvecs,
                                      const cv::Mat& cameraMatrix,
                                      const cv::Mat& distCoeffs,
                                      std::vector<float>& perViewErrors);

    bool runCalibration(cv::Size& imageSize,
                        cv::Mat& cameraMatrix,
                        cv::Mat& distCoeffs,
                        std::vector<std::vector<cv::Point2f> > imagePoints,
                        std::vector<cv::Mat>& rvecs,
                        std::vector<cv::Mat>& tvecs,
                        std::vector<float>& reprojErrs,
                        double& totalAvgErr,
                        std::vector<cv::Point3f>& newObjPoints,
                        float grid_width,
                        bool release_object);

    bool runCalibrationAndSave(cv::Size imageSize,
                               cv::Mat& cameraMatrix,
                               cv::Mat& distCoeffs,
                               std::vector<std::vector<cv::Point2f>> imagePoints,
                               float grid_width,
                               bool release_object);

    void saveCameraParams( Size& imageSize,
                           Mat& cameraMatrix,
                           Mat& distCoeffs,
                           const vector<Mat>& rvecs,
                           const vector<Mat>& tvecs,
                           const vector<float>& reprojErrs,
                           const vector<vector<Point2f> >& imagePoints,
                           double totalAvgErr,
                           const vector<Point3f>& newObjPoints);

    void calibration();
    void readCalibParameters();

private:
    bool m_bRun = false;
    QImage m_qImage;
    bool m_bSave = false;
    bool m_bCalib = false;
    Setting* m_Setting;
    Mat m_map1;
    Mat m_map2;
    Rect m_roi;         //Rectangle of the undistorted image
    bool m_bUndistort = false;
    bool m_dataCalibration = false;
    QVector<int> m_qvWidth = {1920, 1280, 1024, 640, 800, 1280, 320};
    QVector<int> m_qvHeight = {1080, 720, 768, 480, 600, 1024, 240};
    VideoCapture *m_cap;
};

#endif // CAMERATHREAD_H
