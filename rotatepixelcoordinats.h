#ifndef ROTATEPIXELCOORDINATS_H
#define ROTATEPIXELCOORDINATS_H

#include <QPixmap>
#include <QColor>

class RotatePixelCoordinats
{
public:
    RotatePixelCoordinats(const QImage &image);
    virtual ~RotatePixelCoordinats() = default;

    QColor getRotatePixelColor(double angleDegrees, double radius) const;
    QPointF getRotateCoordinats(double angleDegrees, double radius) const; // 'radius' [0; 1]
    double radiusForRotatePixel(int pixelIndex) const;
    double rotatePixelRadius() const; // radius for led pixel

    void setCountPixelInLine(int count);
    int countPixelInLine() const;

    void setIsFirstPixelInSentere(bool isInCenter);
    bool isFirstPixelInSenter() const;

    void setOfsetFromCenter(double ofset);
    double ofsetFromCenter() const;

protected:
    virtual QColor extractColor(const QRectF rect) const;

private:
    double imageRadius() const; // radius for image
    QPointF imageCenter() const;

private:
    QImage _image;

    int _countPixelInLine{10};
    bool _isFirstPixelInSenter{false};
    double _offsetFromCenter{0.0}; //[0; 1]
};

#endif // ROTATEPIXELCOORDINATS_H
