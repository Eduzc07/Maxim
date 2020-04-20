#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <opencv2/opencv.hpp>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    cv::Mat a = cv::imread("cusco.jpg");
    cv::imshow("aaaa",a);
}

MainWindow::~MainWindow()
{
    delete ui;
}
