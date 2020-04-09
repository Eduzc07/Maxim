#include "include/mainwindow.h"
#include "ui_mainwindow.h"

#include <QDebug>
#include <QPixmap>
#include <QTimer>
#include <QPainter>
#include <QDir>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    connect(ui->startButton, SIGNAL(pressed()), this, SLOT(startCamera()));
    connect(ui->stopButton, SIGNAL(pressed()), this, SLOT(stopCamera()));
    connect(ui->saveButton, SIGNAL(pressed()), this, SLOT(saveCamera()));
    connect(ui->calibrateButton, SIGNAL(pressed()), this, SLOT(calibrateCamera()));
    connect(ui->settingButton, SIGNAL(pressed()), this, SLOT(openSetting()));
    connect(ui->cb_undistort, SIGNAL(toggled(bool)), &m_CameraThread, SLOT(setUndistort(bool)));
    connect(&m_CameraThread, SIGNAL(imageReady()), this, SLOT(displayImage()));
    connect(&m_CameraThread, SIGNAL(updateError(double)), this, SLOT(displayError(double)));
    connect(&m_Setting, SIGNAL(accepted()), this, SLOT(readSettings()));

    m_timer = new QTimer(this);
    connect(m_timer, SIGNAL(timeout()), this, SLOT(update()));
    connect(&m_CameraThread, SIGNAL(endLoop()), m_timer, SLOT(stop()));

    QPalette Pal(palette());
    // Asignar el color de fondo como Negro
    Pal.setColor(QPalette::Background, Qt::black);
    ui->imageLabel->setAutoFillBackground(true);
    ui->imageLabel->setPalette(Pal);
    ui->saveButton->setEnabled(false);

    loadEmptyImage();

    //Read Parameters
    readSettings();
//    readCalibParameters();

    ui->cb_undistort->setEnabled(m_CameraThread.isCalibrate());

    //Create folder
    if (!QDir("images").exists())
        QDir().mkdir("images");
}

MainWindow::~MainWindow()
{
    m_CameraThread.stop();
    delete ui;
}

void MainWindow::drawTextImage()
{
    QPainter painter (&m_pixmap);

    QFont font("Arial") ;
    font.setPointSize(80);
    font.setBold(true);

    painter.setFont(font);

    QPen pen;
    pen.setColor(Qt::white);

    QBrush brush;
    brush.setColor(QColor(255, 255, 255, 100));

    painter.setBrush(brush);

    painter.setPen(pen);

    if(!m_timer->isActive() || m_qCount == "0")
        m_qCount = "";

    painter.drawText( QPoint(m_pixmap.width()/2, m_pixmap.height()/2), m_qCount );
}

void MainWindow::startCamera()
{
    m_CameraThread.openCamera();
    m_bStart = true;
    qDebug() << " start";
    m_CameraThread.start();

    ui->calibrateButton->setEnabled(false);
    ui->settingButton->setEnabled(false);
    ui->saveButton->setEnabled(true);
    ui->cb_undistort->setEnabled(m_CameraThread.isCalibrate());
}

void MainWindow::stopCamera()
{
    m_bStart = false;
    qDebug()<< " stop";
    m_CameraThread.stop();
    loadEmptyImage();

    ui->calibrateButton->setEnabled(true);
    ui->settingButton->setEnabled(true);
    ui->saveButton->setEnabled(false);

    //Stop Saving
    m_iCounter = 0;
    m_timer->stop();
}

void MainWindow::saveCamera()
{
    //Delete last Images
    QString path = "./images";
    QDir dir(path);
    dir.setNameFilters(QStringList() << "*.jpg");
    dir.setFilter(QDir::Files);
    foreach(QString dirFile, dir.entryList())
        dir.remove(dirFile);

    ui->cb_undistort->setChecked(false);

    m_iCounter = 5;
    m_timer->start(1000);
}

void MainWindow::calibrateCamera()
{
    m_CameraThread.startCalibration();
    m_bStart = true;
    m_CameraThread.start();

    ui->cb_undistort->setEnabled(false);
}

void MainWindow::displayImage()
{
    if (!m_bStart)
        return;

    QImage image = m_CameraThread.getImage();
    m_pixmap = QPixmap::fromImage(image, Qt::AutoColor);

    drawTextImage();

    int w = ui->imageLabel->width();
    int h = ui->imageLabel->height();

    ui->imageLabel->setPixmap(m_pixmap.scaled(w, h,Qt::KeepAspectRatio));
//    ui->imageLabel->setPixmap(m_pixmap);
}

void MainWindow::displayError(double value)
{
    ui->label_error->setText(QString::number(value, 'f', 4));
    if (value <= 1.0)
        ui->label_error->setStyleSheet("font-weight: bold; color: green");
    else
        ui->label_error->setStyleSheet("font-weight: bold; color: red");
}

void MainWindow::loadEmptyImage()
{
    QImage green;
    green.load(":/images/nocamera.jpg");

    if(green.isNull())
        qDebug()<<"isEmpty";

    QPixmap qpix = QPixmap::fromImage(green, Qt::AutoColor);
    ui->imageLabel->setPixmap(qpix);
    ui->imageLabel->setAlignment (Qt::AlignCenter);
}

void MainWindow::openSetting()
{
    m_Setting.open();
}

void MainWindow::readSettings()
{
    m_CameraThread.loadSetting(&m_Setting);
}

void MainWindow::update()
{
    m_qCount = QString::number(m_iCounter);

    m_iCounter--;
    if (m_iCounter < 0) {
        m_iCounter = 5;
        m_CameraThread.save();
    }
}
