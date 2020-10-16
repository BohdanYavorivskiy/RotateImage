#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <rotatepixelcoordinats.h>

#include <QMainWindow>
#include <QPixmap>
#include <QColor>

#include <QVector>

#include <memory>

class QScrollArea;
class QLabel;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow() = default;

    void loadImage(const QString &path);

private:
    QPixmap createRotateImage();

    QVector<QColor> dataFromPixel(const QVector<QColor> &pixels, int brightness);
    void printLine(const QVector<QColor> data, int lineIndex);
    void printHead(int lineCount, int lineLength);
    void setupUi();
    void showImage();

private:
    QImage _image;
    std::unique_ptr<RotatePixelCoordinats> _rotate{nullptr};
// ui
    QScrollArea *_scrollArea;
    QLabel *_imageViewer;
    QLabel *_rotateImageViewer;
};

#endif // MAINWINDOW_H
