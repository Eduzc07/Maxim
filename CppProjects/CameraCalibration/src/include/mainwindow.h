#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

#include "camerathread.h"
#include "Setting.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    Ui::MainWindow *ui;
    CameraThread m_CameraThread;

    void drawTextImage();

public slots:
    void startCamera();
    void stopCamera();
    void saveCamera();
    void calibrateCamera();
    void displayImage();
    void update();
    void loadEmptyImage();
    void openSetting();
    void readSettings();

private:
    bool m_bStart = false;
    int m_iCounter = 0;
    QTimer *m_timer;
    QString m_qCount;
    QPixmap m_pixmap;
    Setting m_Setting;
};

#endif // MAINWINDOW_H
