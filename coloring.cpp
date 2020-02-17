#include "coloring.h"

#define PI 3.14159265359

namespace coloring{      
    // diffrent types of coloring


    QRgb iter_to_circle(int iterations){
        int x = (iterations % 180) * 2;
        QColor color = QColor::fromHsv(x, 255, 155);

        return qRgb(color.red(), color.green(), color.blue());
    }



    QRgb zick_zack_rgb(int iterations){
        const int numOfColors = 3 * 18;
        const int numOfSteps = 18;

        iterations += 0;
        int r, g, b;
        int range = 255*(iterations%numOfSteps + 1)/(numOfSteps);
        switch( (iterations % numOfColors) / numOfSteps)
        {
            case 0:
                r = range;
                g = 255 - range;
                b = 0;
            break;
            case 1:
                b = range;
                r = 255 -range;
                g = 0;
            break;
            default:
                g = range;
                b = 255 - range;
                r = 0;
        }

        return qRgb(r, g, b);
    }


    QRgb colored_by_argument(std::complex<double> p){
        int arg = int(std::arg(p)*180.0/PI) + 180;
        QColor color = QColor::fromHsv(arg, 255, 155);
        return qRgb(color.red(), color.green(), color.blue());
    }

    QRgb black_white_distance(double dist){
        int intensity = int(255.0 * dist / 2.0);
        return qRgb(intensity,intensity,intensity);
    }

    QRgb colored_orbit(double dist, std::complex<double> p){
        int value = int(255.0 * dist / 2.0);
        int arg = int(std::arg(p)*180.0/PI) + 180;
        QColor color = QColor::fromHsv(arg, 255, value);
        return qRgb(color.red(), color.green(), color.blue());
    }
}

#undef PI
