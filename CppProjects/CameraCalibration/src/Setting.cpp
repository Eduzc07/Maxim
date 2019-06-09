#include "include/Setting.h"
#include "ui_Setting.h"

#include "QDebug"

Setting::Setting(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Setting)
{
    ui->setupUi(this);
    connect(ui->pushButton, SIGNAL(pressed()), this, SLOT(readValues()));
    loadDefault();

    //Save all
    writePoints = true;
    writeExtrinsics = true;
    writeGrid = true;
}

Setting::~Setting()
{
    delete ui;
}

void Setting::write(FileStorage &fs) const
{
    fs << "{"
       << "BoardSize_Width"  << boardSize.width
       << "BoardSize_Height" << boardSize.height
       << "Square_Size"      << squareSize
       << "Calibrate_Pattern" << patternToUse.toUtf8().constData()
       << "Calibrate_NrOfFrameToUse" << nrFrames
       << "Calibrate_FixAspectRatio" << aspectRatio
       << "Calibrate_AssumeZeroTangentialDistortion" << calibZeroTangentDist
       << "Calibrate_FixPrincipalPointAtTheCenter" << calibFixPrincipalPoint

//       << "Write_DetectedFeaturePoints" << writePoints
//       << "Write_extrinsicParameters"   << writeExtrinsics
//       << "Write_gridPoints" << writeGrid
//       << "Write_outputFileName"  << outputFileName

//       << "Show_UndistortedImage" << showUndistorsed

       << "Input_FlipAroundHorizontalAxis" << flipVertical
//       << "Input_Delay" << delay
//       << "Input" << input
       << "}";
}



void Setting::readValues()
{
    cameraID = ui->sb_camera->value();

    int width = ui->sB_Width->value();
    int height = ui->sB_Height->value();
    boardSize = Size(width, height);

    calibrationPattern = NOT_EXISTING;
    switch(ui->comboBox->currentIndex()){
    case 0:
        calibrationPattern = CHESSBOARD;
        break;
    case 1:
        calibrationPattern = CIRCLES_GRID;
        break;
    case 2:
        calibrationPattern = ASYMMETRIC_CIRCLES_GRID;
        break;
    default:
        calibrationPattern = CHESSBOARD;
        break;
    }
    squareSize = ui->sB_Square->value();
    nrFrames = ui->sB_frames->value();
    aspectRatio = ui->ds_ratio->value();
    calibZeroTangentDist = ui->cb_tangential->isChecked();
    calibFixPrincipalPoint = ui->cb_center->isChecked();
    flipVertical = ui->cb_flip->isChecked();
    useFisheye = ui->cb_fisheye->isChecked();
    fixK1 = ui->cb_k1->isChecked();
    fixK2 = ui->cb_k2->isChecked();
    fixK3 = ui->cb_k3->isChecked();
    fixK4 = ui->cb_k4->isChecked();
    fixK5 = ui->cb_k5->isChecked();
    outputFileName = "images/" + ui->lineEdit->text();
    removeBorder = ui->cb_Border->isChecked();
    validate();
}

void Setting::loadDefault()
{
    boardSize = Size(9, 7);
    calibrationPattern = CHESSBOARD;
    squareSize = 20;
    nrFrames = 20;
    aspectRatio = 1.0;
    calibZeroTangentDist = true;
    calibFixPrincipalPoint = true;
    flipVertical = false;
    useFisheye = false;
    fixK1 = false;
    fixK2 = false;
    fixK3 = false;
    fixK4 = true;
    fixK5 = true;
    removeBorder = false;
    outputFileName = "out_camera_data.xml";

    // Find feature points on the input format
    switch(calibrationPattern){
    case CHESSBOARD:
        ui->comboBox->setCurrentIndex(0);
        patternToUse = "CHESSBOARD";
        break;
    case CIRCLES_GRID:
        ui->comboBox->setCurrentIndex(1);
        patternToUse = "CIRCLES_GRID";
        break;
    case ASYMMETRIC_CIRCLES_GRID:
        ui->comboBox->setCurrentIndex(2);
        patternToUse = "ASYMMETRIC_CIRCLES_GRID";
        break;
    default:
        ui->comboBox->setCurrentIndex(0);
        patternToUse = "CHESSBOARD";
        break;
    }

    cameraID = 0;
    ui->sB_Width->setValue(cameraID);

    //Number of inner corners per a item row and column. (square, circle)
    ui->sB_Width->setValue(boardSize.width);
    ui->sB_Height->setValue(boardSize.height);
    //The size of a square in some user defined metric system (pixel, millimeter)
    ui->sB_Square->setValue(squareSize);
    //How many frames to use, for calibration.
    ui->sB_frames->setValue(nrFrames);
    //If true (non-zero) we flip the input images around the horizontal axis.
    ui->cb_flip->setChecked(flipVertical);
    //Consider only fy as a free parameter, the ratio fx/fy stays the same as
    // in the input cameraMatrix. Use or not setting. 0 - False Non-Zero - True
    ui->ds_ratio->setValue(aspectRatio);
    //If true (non-zero) the principal point is not changed during the global optimization.
    ui->cb_center->setChecked(calibFixPrincipalPoint);
    //If true (non-zero) tangential distortion coefficients  are set to zeros and stay zero.
    ui->cb_tangential->setChecked(calibZeroTangentDist);
    //If true (non-zero) will be used fisheye camera model.
    ui->cb_fisheye->setChecked(useFisheye);
    ui->cb_fisheye->setEnabled(false);
    //If true (non-zero) distortion coefficient k1 will be equals to zero.
    ui->cb_k1->setChecked(fixK1);
    //If true (non-zero) distortion coefficient k2 will be equals to zero.
    ui->cb_k2->setChecked(fixK2);
    //If true (non-zero) distortion coefficient k3 will be equals to zero.
    ui->cb_k3->setChecked(fixK3);
    //If true (non-zero) distortion coefficient k4 will be equals to zero.
    ui->cb_k4->setChecked(fixK4);
    //If true (non-zero) distortion coefficient k5 will be equals to zero.
    ui->cb_k5->setChecked(fixK5);

    //Remove Borders
    ui->cb_Border->setChecked(removeBorder);

    //The name of the output log file.
    ui->lineEdit->setText(outputFileName);
    outputFileName = "images/" + ui->lineEdit->text();
}

void Setting::validate()
{
    flag = 0;
    if(calibFixPrincipalPoint)          flag |= CALIB_FIX_PRINCIPAL_POINT;
    if(calibZeroTangentDist)            flag |= CALIB_ZERO_TANGENT_DIST;
    if(aspectRatio==1.0?true:false)     flag |= CALIB_FIX_ASPECT_RATIO;
    if(fixK1)                           flag |= CALIB_FIX_K1;
    if(fixK2)                           flag |= CALIB_FIX_K2;
    if(fixK3)                           flag |= CALIB_FIX_K3;
    if(fixK4)                           flag |= CALIB_FIX_K4;
    if(fixK5)                           flag |= CALIB_FIX_K5;

    if (useFisheye) {
        // the fisheye model has its own enum, so overwrite the flags
        flag = fisheye::CALIB_FIX_SKEW | fisheye::CALIB_RECOMPUTE_EXTRINSIC;
        if(fixK1)                   flag |= fisheye::CALIB_FIX_K1;
        if(fixK2)                   flag |= fisheye::CALIB_FIX_K2;
        if(fixK3)                   flag |= fisheye::CALIB_FIX_K3;
        if(fixK4)                   flag |= fisheye::CALIB_FIX_K4;
        if (calibFixPrincipalPoint) flag |= fisheye::CALIB_FIX_PRINCIPAL_POINT;
    }
}
