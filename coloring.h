#ifndef COLORING_H
#define COLORING_H

#include <QImage>
#include <QtXml/QDomDocument>
#include <QFile>
#include <windows.h>
#include <QtCore>
#include <vector>
#include <complex>

namespace coloring{
    enum colorfunction{
        wavelength,
        zickZackRGB,
        cieInnercircle
    };

    enum drawingfunction{
        iteration,
        innerColoring,
        coloredArgument,
        orbitTrapping
    };

    QRgb colored_by_argument(std::complex<double> p);
    QRgb cie_innercircle(int iterations);
    QRgb zick_zack_rgb(int iterations);
    QRgb wavelength_color(int iterations);
    QRgb colored_c_plane(std::complex<double> p);
    QRgb black_white_distance(double dist);
    QRgb colored_orbit(double dist, std::complex<double> p);
}

#endif // COLORING_H
