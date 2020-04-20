#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    QWidget::move(50,50);


    example *pos=ui->graphicsView;
    connect(pos, SIGNAL(valueChanged(QPoint,QPoint)),this,SLOT(Receive_point(QPoint,QPoint)));

//    QString fileName = QFileDialog::getOpenFileName(this,"Open Image File",QDir::currentPath());
    QString fileName = "/home/edu/Pictures/androidparty.jpg";

    if(!fileName.isEmpty())
    {
        QImage image(fileName);

        if(image.isNull())
        {
            QMessageBox::information(this,"Image Viewer","Error Displaying image");
            return;
        }

        image2 = QPixmap::fromImage(image);
//        int w=ui->graphicsView->width()-2;
//        int h=ui->graphicsView->height()-2;

//        item = new QGraphicsPixmapItem(image2.scaled(480,640,Qt::KeepAspectRatio));
        item = new QGraphicsPixmapItem(image2);
        scene =new QGraphicsScene(this);

        ui->graphicsView->setScene(scene);
        int w=image2.width();
        int h=image2.height();
        scene->setSceneRect(0, 0,w,h);
        scene->addItem(item);

    }

   QGraphicsScene *scene3=new QGraphicsScene(this);

   //Chart
   QGraphicsSimpleTextItem *m_coordX;
   QGraphicsSimpleTextItem *m_coordY;
   QChart *m_chart;

   // chart
   m_chart = new QChart;
   m_chart->setMinimumSize(640, 480);
   m_chart->setAnimationOptions(QChart::SeriesAnimations);
//    QLineSeries *series = new QLineSeries;
   QSplineSeries *series = new QSplineSeries;

//   for(int i=0; i<plot_wave.size();i++){
//       series->append(plot_wave[i][0],plot_wave[i][1]);
//   }

   series->append(500, 500);
   series->append(600, 200);
   series->append(750, 100);
   m_chart->addSeries(series);

   // Customize series
   QPen pen(QColor(15,200,15));
   pen.setWidth(3);
   series->setPen(pen);

   m_chart->createDefaultAxes();
   m_chart->setAcceptHoverEvents(true);

   QValueAxis *axisX = new QValueAxis;
   QValueAxis *axisY = new QValueAxis;

   // Customize axis label font
   QFont labelsFont;
   labelsFont.setPixelSize(12);
   axisX->setLabelsFont(labelsFont);

   axisX->setRange(400, 800);
//    axisX->setLabelFormat("%g");
   axisX->setTitleBrush(Qt::white);
   axisX->setTitleText("Wavelength [nm]");


   axisY->setLabelsFont(labelsFont);
   axisY->setRange(0, 1200);
   axisY->setLabelFormat("%g");
   axisY->setTitleBrush(Qt::white);
   axisY->setTitleText("Counts");

   // Customize axis colors
   QPen axisPen(QRgb(0xd18952));
   axisPen.setWidth(2);
   axisX->setLinePen(axisPen);
   axisY->setLinePen(axisPen);

   // Customize axis label colors
   QBrush axisBrush(Qt::white);
   axisX->setLabelsBrush(axisBrush);
   axisY->setLabelsBrush(axisBrush);

   // Customize grid lines and shades
   //axisX->setGridLineVisible(false);
   axisY->setGridLineVisible(false);
   axisY->setShadesPen(Qt::NoPen);
   axisY->setShadesBrush(QBrush(QColor(0x99, 0xcc, 0xcc, 0x55)));
   axisY->setShadesVisible(true);


   m_chart->setAxisX(axisX, series);
   m_chart->setAxisY(axisY, series);
   m_chart->legend()->hide();


   // Customize chart title
   QFont font;
   font.setPixelSize(18);
   m_chart->setTitleFont(font);
   m_chart->setTitleBrush(QBrush(Qt::white));
   m_chart->setTitle("Spectrum");

   // Customize chart background
   QLinearGradient backgroundGradient;
   backgroundGradient.setStart(QPointF(0, 0));
   backgroundGradient.setFinalStop(QPointF(0, 1));
   backgroundGradient.setColorAt(0.0, QRgb(0xd2d0d1));
   backgroundGradient.setColorAt(1.0, QRgb(0x4c4547));
   backgroundGradient.setCoordinateMode(QGradient::ObjectBoundingMode);
   m_chart->setBackgroundBrush(backgroundGradient);

   // Customize plot area background
   QLinearGradient plotAreaGradient;
   plotAreaGradient.setStart(QPointF(0, 1));
   plotAreaGradient.setFinalStop(QPointF(1, 0));
   plotAreaGradient.setColorAt(0.0, QRgb(0x555555));
   plotAreaGradient.setColorAt(1.0, QRgb(0x55aa55));
   plotAreaGradient.setCoordinateMode(QGradient::ObjectBoundingMode);
   m_chart->setPlotAreaBackgroundBrush(plotAreaGradient);
   m_chart->setPlotAreaBackgroundVisible(true);


   m_coordX = new QGraphicsSimpleTextItem(m_chart);
   m_coordX->setPos(m_chart->size().width()+30, m_chart->size().height()+15);
   m_coordX->setText("Wavelength [nm]: ");
   m_coordY = new QGraphicsSimpleTextItem(m_chart);
   m_coordY->setPos(m_chart->size().width()+30, m_chart->size().height()+30);
   m_coordY->setText("Counts: ");

   QChartView *chartView = new QChartView(m_chart);
   chartView->setRenderHint(QPainter::Antialiasing);
   chartView->setMinimumSize(640,480);

   ui->graphicsView_2->setScene(scene3);
//   scene3->setSceneRect(0, 0,640,480);
   scene3->addItem(m_chart);
//    ui->graphicsView->show();
    qDebug() << "Start..." ;
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_action_Help_triggered()
{
    QMessageBox::aboutQt(this,"Qt");
}

void MainWindow::on_pushButton_clicked()
{
     QApplication::quit();
}

void MainWindow::on_action_Exit_triggered()
{
    QApplication::quit();
}

void MainWindow::Receive_point(QPoint pi,QPoint pf)
{
  qDebug() << "Received...pi" << pi;
  qDebug() << "Received...pf" << pf;

  QRect rect(pi.x(), pi.y(), pf.x(),pf.y());
  QPixmap cropped = image2.copy(rect);

  QGraphicsItem *item2= new QGraphicsPixmapItem(cropped);
  QGraphicsScene *scene2 =new QGraphicsScene();
  ui->graphicsView_3->setScene(scene2);

  int w=cropped.width();
  int h=cropped.height();
  scene2->setSceneRect(0, 0,w,h);
  scene2->addItem(item2);
}
