#ifndef SETTING_H
#define SETTING_H

#include <iostream>
#include <sstream>
#include <string>
#include <ctime>
#include <cstdio>

#include <QDialog>
#include <QDebug>

#include <opencv2/core.hpp>
#include <opencv2/core/utility.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/calib3d.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/videoio.hpp>
#include <opencv2/highgui.hpp>

namespace Ui {
class Setting;
}

using namespace cv;
using namespace std;

class Setting : public QDialog
{
    Q_OBJECT

public:
    explicit Setting(QWidget *parent = nullptr);
    ~Setting();
    enum Pattern { NOT_EXISTING, CHESSBOARD, CIRCLES_GRID, ASYMMETRIC_CIRCLES_GRID };
    void write(FileStorage& fs) const;

public:
    int cameraID;
    Size boardSize;              // The size of the board -> Number of items by width and height
    Pattern calibrationPattern;  // One of the Chessboard, circles, or asymmetric circle pattern
    int squareSize;              // The size of a square in your defined unit (point, millimeter,etc).
    int nrFrames;                // The number of frames to use from the input for calibration
    double aspectRatio;           // The aspect ratio
    int delay;                   // In case of a video input
    bool writePoints;            // Write detected feature points
    bool writeExtrinsics;        // Write extrinsic parameters
    bool writeGrid;              // Write refined 3D target grid points
    bool calibZeroTangentDist;   // Assume zero tangential distortion
    bool calibFixPrincipalPoint; // Fix the principal point at the center
    bool flipVertical;           // Flip the captured images around the horizontal axis
    QString outputFileName;       // The name of the file where to write
    bool showUndistorsed;        // Show undistorted images after calibration
    QString input;                // The input ->
    bool useFisheye;             // use fisheye camera model for calibration
    bool fixK1;                  // fix K1 distortion coefficient
    bool fixK2;                  // fix K2 distortion coefficient
    bool fixK3;                  // fix K3 distortion coefficient
    bool fixK4;                  // fix K4 distortion coefficient
    bool fixK5;                  // fix K5 distortion coefficient

    int flag;
    bool removeBorder;

private slots:
    void readValues();

private:
    void loadDefault();
    void validate();

private:
    Ui::Setting *ui;
    QString patternToUse;
};

#endif // SETTING_H
