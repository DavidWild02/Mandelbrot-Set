#ifndef COLORING_H
#define COLORING_H

#include <QImage>
#include <QtXml/QDomDocument>
#include <QFile>
#include <windows.h>
#include <QtCore>
#include <vector>
#include <complex>
#include <QtDebug>

namespace coloring{
    enum colorfunction{
        zickZackRGB,
        circleHSV
    };

    enum drawingfunction{
        iteration,
        innerColoring,
        coloredArgument,
        orbitTrapping
    };

    QRgb iter_to_circle(int iterations);
    QRgb zick_zack_rgb(int iterations);
    QRgb black_white_distance(double dist);
    QRgb colored_by_argument(std::complex<double> p);
    QRgb colored_orbit(double dist, std::complex<double> p);
}

#endif // COLORING_H
