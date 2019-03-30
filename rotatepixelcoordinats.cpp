#include "rotatepixelcoordinats.h"

#include <QtMath>

#include <QDebug>

RotatePixelCoordinats::RotatePixelCoordinats(const QImage &image): _image(image)
{

}

QColor RotatePixelCoordinats::getRotatePixelColor(double angleDegrees, double radius) const
{
    const double pixelRadius {rotatePixelRadius()};
    QPointF coordinats { getRotateCoordinats(angleDegrees, radius)};
    return extractColor(QRectF(coordinats.x() - pixelRadius, coordinats.y() - pixelRadius, pixelRadius * 2, pixelRadius * 2));
}

QPointF RotatePixelCoordinats::getRotateCoordinats(double angleDegrees, double radius) const
{
    const double imageRadInPixels {imageRadius()};
    QPointF imgCenter {imageCenter()};

    // get x
    double x{qCos(qDegreesToRadians(angleDegrees)) * radius};
    // get y
    double y{qSin(qDegreesToRadians(angleDegrees)) * radius};

    // map x to image
    x = x * imageRadInPixels;
    x += imgCenter.x();

    // map y to image
    y = y * imageRadInPixels;
    y += imgCenter.y();

    return QPointF(x, y);
}

QColor RotatePixelCoordinats::extractColor(const QRectF rect) const
{
    int firstX = std::ceil(rect.x());
    int lastX = std::trunc(rect.x() + rect.width());
    int firstY = std::ceil(rect.y());
    int lastY = std::trunc(rect.y() + rect.height());

    if (firstX < 0)
    {
//        qDebug("firstX < 0");
        firstX = 0;
    }

    if (firstX >= _image.width())
    {
//        qDebug("firstX >= _image.width()");
        firstX = _image.width() - 1;
    }


    if (lastX < 0)
    {
//        qDebug("lastX < 0");
        lastX = 0;
    }

    if (lastX >= _image.width())
    {
//        qDebug("lastX >= _image.width()");
        lastX = _image.width() - 1;
    }

    if (firstY < 0)
    {
//        qDebug("firstY < 0");
        firstY = 0;
    }

    if (firstY >= _image.height())
    {
//        qDebug("firstY >= _image.width()");
        firstY = _image.width() - 1;
    }

    if (lastY < 0)
    {
//        qDebug("lastY < 0");
        lastY = 0;
    }

    if (lastY >= _image.height())
    {
//        qDebug("lastY >= _image.width()");
        lastY = _image.width() - 1;
    }

    int rSum = 0;
    int gSum = 0;
    int bSum = 0;

    //    for(int i = firstX; i <= lastX; ++i)
    //    {
    //        for(int j = firstY; j <= lastY; ++j)
    //        {
    //            rSum += _image.pixelColor(i, j).red();
    //            gSum += _image.pixelColor(i, j).green();
    //            bSum += _image.pixelColor(i, j).blue();

    //            ++pixelCount;
    //        }
    //    }

    QPointF center;
    center.rx() = rect.x() + rect.width() / 2.0;
    center.ry() = rect.y() + rect.height() / 2.0;

    const double currentRad{(rect.width() / 2.0) * 0.3};

    double counPixelF = 0;
    for(int i = firstX; i <= lastX; ++i)
    {
        for(int j = firstY; j <= lastY; ++j)
        {
            const double distanceX { qFabs(qFabs(i) - qFabs(center.x())) };
            const double distanceY { qFabs(qFabs(j) - qFabs(center.y())) };
            const double distance { qSqrt(qPow(distanceX, 2) + qPow(distanceY, 2)) };
            if (distance > currentRad)
                continue;

            const double distMultipler { (currentRad - distance) / currentRad };

            rSum += _image.pixelColor(i, j).red() * distMultipler;
            gSum += _image.pixelColor(i, j).green() * distMultipler;
            bSum += _image.pixelColor(i, j).blue() * distMultipler;

            counPixelF += distMultipler;
        }
    }


    return QColor(rSum / counPixelF, gSum / counPixelF, bSum / counPixelF);
}

double RotatePixelCoordinats::imageRadius() const
{
    double radLenInPixels{0};
    if (_image.width() < _image.height())
        radLenInPixels = _image.width() / 2.0;
    else
        radLenInPixels = _image.height() / 2.0;

    return radLenInPixels;
}

double RotatePixelCoordinats::rotatePixelRadius() const
{
    return  imageRadius() * (1.0 / (static_cast<double>(_countPixelInLine) * 2.0));
}

void RotatePixelCoordinats::setCountPixelInLine(int count)
{
    _countPixelInLine = count;
}

int RotatePixelCoordinats::countPixelInLine() const
{
    return _countPixelInLine;
}

void RotatePixelCoordinats::setIsFirstPixelInSentere(bool isInCenter)
{
    _isFirstPixelInSenter = isInCenter;
}

bool RotatePixelCoordinats::isFirstPixelInSenter() const
{
    return _isFirstPixelInSenter;
}

void RotatePixelCoordinats::setOfsetFromCenter(double offset)
{
    _offsetFromCenter = offset;
}

double RotatePixelCoordinats::ofsetFromCenter() const
{
    return _offsetFromCenter;
}

double RotatePixelCoordinats::radiusForRotatePixel(int pixelIndex) const
{
    const double senterShifter {_isFirstPixelInSenter ? 0.5 : 0.0};
    const double pixelLineShifter {(_isFirstPixelInSenter ? 0.0 : ((1.0 / _countPixelInLine) * 0.5))};
    const double shiftForOnePixel {(1.0 / ((static_cast<double>(_countPixelInLine) - senterShifter)))};
    return ((shiftForOnePixel * (1 - _offsetFromCenter)) * static_cast<double>(pixelIndex)) + pixelLineShifter + _offsetFromCenter;
}

QPointF RotatePixelCoordinats::imageCenter() const
{
    return QPointF(_image.width() / 2.0, _image.height() / 2.0);
}
