#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    image = NULL;
    ui->setupUi(this);
    connect(ui->openButton, SIGNAL(pressed()), this, SLOT(OpenFile()));
    connect(ui->processButton, SIGNAL(pressed()), this, SLOT(ProcessFile()));
}

MainWindow::~MainWindow()
{
    delete ui;
}

float MainWindow::is_green()
{
    int w = image->width();
    int h = image->height();

    int nbPixel = 0;
    int count = 0;
    for(int i =0; i < w; i++)
    {
        for(int j = 0; j < h; j++)
        {
            QRgb pixel = image->pixel(i, j);
            int r = qRed(pixel);
            int g = qGreen(pixel);
            int b = qBlue(pixel);

            if(g > r && g > b)
                nbPixel++;
            count++;
            ui->progressBar->setValue(count);
        }
    }
    return (nbPixel / (w*h)) * 100;
}

void MainWindow::OpenFile()
{
    // OPEN BUTTON CLICK

    QString fileName = QFileDialog::getOpenFileName(this, tr("Open Image"), "", tr("Image Files (*.png *.jpg *.bmp)"));
    path = fileName.toStdString();

    if(path != "")
    {
        image = new QImage(fileName);
        QImage image2 = image->scaled(200, 200, Qt::AspectRatioMode::KeepAspectRatio);
        ui->imageViewer->setPixmap(QPixmap::fromImage(image2));

        ui->statusLabel->setText(tr("No Process"));
    }
    ui->processButton->setEnabled(true);
}

void MainWindow::ProcessFile()
{
    // PROCESS BUTTON CLICK
    if(path == "") return;

    ui->progressBar->setMaximum(image->size().width() * image->size().height());

    float percentage = is_green();

    std::ostringstream ss;
    ss << "There are " << percentage << " % in this image.";

    ui->statusLabel->setText(tr(ss.str().c_str()));
}
