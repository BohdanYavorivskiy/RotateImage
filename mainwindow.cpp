#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QImageReader>
#include <QMessageBox>
#include <QDir>
#include <QtMath>
#include <QPen>
#include <QHBoxLayout>
#include <QPainter>
#include <QVector>

#include <math.h>

#include <QDebug>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent)
{
    setupUi();/*apple.jpg*/ /*test.png*/ /*pacman.jpg*/ /*packman_1.jpg*/
    loadImage("/home/bohdan/Pictures/lisa-simpson-lisa-simpson-640542_500_500.jpg");
    showImage();

    _rotateImageViewer->setPixmap(createRotateImage());
}

void MainWindow::loadImage(const QString &path)
{
    QImageReader reader(path);
    reader.setAutoTransform(true);
    const QImage newImage = reader.read().convertToFormat(QImage::Format_RGB888);
    if (newImage.isNull())
        QMessageBox::information(this, QGuiApplication::applicationDisplayName(),
                                 tr("Cannot load %1: %2")
                                 .arg(QDir::toNativeSeparators(path), reader.errorString()));

    _image = newImage;
    _rotate.reset(new RotatePixelCoordinats(_image));
}

QPixmap MainWindow::createRotateImage()
{
    QPixmap pixmap(_image.width(), _image.height());
    pixmap.fill();
    QPainter painter(&pixmap);
    painter.setPen(QPen(QColor(0, 0, 0, 0)));

    if (!_rotate)
        return pixmap;

    const double sectionCont {100};
    const double radElement {_rotate->rotatePixelRadius()};

    printHead(sectionCont, _rotate->countPixelInLine() - 1);

    for(double v = 0; v < sectionCont; ++v)
    {
        QVector<QColor> lineLed;
        for(int i = 0; i < _rotate->countPixelInLine(); ++i)
        {
            QColor color {_rotate->getRotatePixelColor((360.0 / sectionCont) * v, _rotate->radiusForRotatePixel(i)) };
            lineLed.push_back(color);
//            color.setAlpha(50);
            QBrush brush = painter.brush();
            brush.setStyle(Qt::SolidPattern);
            brush.setColor(color);
            painter.setBrush(brush);

            QPointF rotateCoordinats {_rotate->getRotateCoordinats((360.0 / sectionCont) * v, _rotate->radiusForRotatePixel(i))};
            painter.drawEllipse(rotateCoordinats, radElement * 0.5, radElement * 0.5);
        }

        QVector<quint8> data = dataFromPixel(lineLed, 9);
        printLine(data, v);
    }
    return pixmap;
}

QVector<quint8> MainWindow::dataFromPixel(const QVector<QColor> &pixels, int brightness)
{
    QVector<quint8> data;

    const double brightnessCoef {1.11};

    const double brightnessBase { (brightness / brightnessCoef) * (brightnessCoef - 1)};//it's min value for brightness
    for(int i = 0; i < pixels.count(); ++i)
    {
        const double brightnessMultipler = brightnessBase + (brightness / brightnessCoef) * ((i + 1) / static_cast<double>(pixels.count()));

        data.push_back(qGreen(pixels[i].rgb()) * (brightnessMultipler / 100.0));
        data.push_back(qRed(pixels[i].rgb()) * (brightnessMultipler / 100.0));
        data.push_back(qBlue(pixels[i].rgb()) * (brightnessMultipler / 100.0));
    }
    return data;
}

void MainWindow::printLine(const QVector<quint8> data, int lineIndex)
{
    QString s;
    for(int i = 0; i < data.size(); ++i)
        s += QString("0x%2, ").arg(QString::number(data[i], 16));
//        s += QString("*(arrayPix + (%0 * lineLength + %1)) = 0x%2; ").arg(lineIndex).arg(i).arg(QString::number(data[i], 16));
    qDebug().noquote() << s;
}

void MainWindow::printHead(int lineCount, int lineLength)
{
    qDebug().noquote() << QString("const int lineCount {%0};").arg(lineCount);
    qDebug().noquote() << QString("const int lineLength {%0};").arg(lineLength * 3 - 1);
    qDebug() << "uint8_t arrayPix[lineCount * lineLength];";
}


void MainWindow::setupUi()
{
    _imageViewer = new QLabel;
    _imageViewer->adjustSize();
    _imageViewer->setBackgroundRole(QPalette::Base);
    _imageViewer->setSizePolicy(QSizePolicy::Ignored, QSizePolicy::Ignored);
    _imageViewer->setScaledContents(true);

    _rotateImageViewer = new QLabel;
    _rotateImageViewer->adjustSize();
    _rotateImageViewer->setBackgroundRole(QPalette::Base);
    _rotateImageViewer->setSizePolicy(QSizePolicy::Ignored, QSizePolicy::Ignored);
    _rotateImageViewer->setScaledContents(true);

    _scrollArea = new QScrollArea(this);
    _scrollArea->setBackgroundRole(QPalette::Dark);
    _scrollArea->setVisible(true);

    QHBoxLayout *hLayout = new QHBoxLayout;
    hLayout->addWidget(_imageViewer);
    hLayout->addWidget(_rotateImageViewer);

    _scrollArea->setLayout(hLayout);

    setCentralWidget(_scrollArea);
}

void MainWindow::showImage()
{
    _imageViewer->setPixmap(QPixmap::fromImage(_image));
    _imageViewer->adjustSize();
}
