#include "coloring.h"

namespace coloring{
    namespace{
        /*
         * I used the information on this website, for getting a high quality function to render the wavelength spectra into rgb
         * https://www.fourmilab.ch/documents/specrend/
         * https://www.fourmilab.ch/documents/specrend/specrend.c
         */

        // loading the data for the colourtongue into an array
        static std::vector<std::vector<float>> cie_colour_match = [](){
            TCHAR path[MAX_PATH + 1] = L"";
            GetCurrentDirectory(MAX_PATH, path);
            std::wstring directory(&path[0]);

            QString filename(QString::fromStdWString(directory) + "\\Data\\cie_color_function.xml");
            QDomDocument doc;
            QFile xmlFile(filename);

            if (!xmlFile.open(QIODevice::ReadOnly) || !doc.setContent(&xmlFile))
                qDebug() << "Data not accesible\n";

            std::vector<std::vector<float>> cie_colour_match;

            QDomNodeList records = doc.elementsByTagName("Record");
            for(size_t i = 0; i < size_t(records.size()); i++){
                QDomNode record = records.item(int(i));
                QDomElement r = record.firstChildElement("Field2");
                QDomElement g = record.firstChildElement("Field3");
                QDomElement b = record.firstChildElement("Field4");
                cie_colour_match.push_back(std::vector<float>());
                cie_colour_match[i].push_back(r.text().toFloat());
                cie_colour_match[i].push_back(g.text().toFloat());
                cie_colour_match[i].push_back(b.text().toFloat());
            }

            return cie_colour_match;
        }();


        /*                  Name    xRed    yRed    xGreen  yGreen  xBlue   yBlue   White point        Gamma   */
        //SMPTEsystem =  { "SMPTE", 0.630,  0.340,  0.310,  0.595,  0.155,  0.070,  0.3127, 0.3291,  GAMMA_REC709 }
        const float xr = 0.630f; const float xg = 0.310f; const float xb = 0.155f;
        const float yr = 0.340f; const float yg = 0.595f; const float yb = 0.070f;
        const float zr = 1 - (xr+yr); const float zg = 1 - (xg+yg); const float zb = 1 - (xb+yb);

        const float xw = 0.3127f; const float yw = 0.3291f;
        const float zw = 1 - (xw + yw);


        float calculate_x_y(std::vector<float> fv, int n){
            return fv[size_t(n)] / (fv[0] + fv[1] + fv[2]);
        }


        static int cie_len = int (cie_colour_match.size());
        static float disX = std::abs(calculate_x_y(cie_colour_match[0], 0) - calculate_x_y(cie_colour_match.back(), 0));
        static float disY = std::abs(calculate_x_y(cie_colour_match[0], 1) - calculate_x_y(cie_colour_match.back(), 1));


        // for wavelength_color, will be deleted soon
        static int times = [](){
            float sumx, sumy;
            for(size_t i = 0; i < size_t(cie_len - 1); i++){
                sumx += calculate_x_y(cie_colour_match[i], 0) - calculate_x_y(cie_colour_match[i + 1], 0);
                sumy += calculate_x_y(cie_colour_match[i], 1) - calculate_x_y(cie_colour_match[i + 1], 1);
            }
            float meanx, meany;
            meanx = sumx/cie_len;
            meany = sumy/cie_len;

            float l = float (std::sqrt(std::pow(meanx, 2) + std::pow(meany, 2)));
            float dis = float (std::sqrt(std::pow(disX, 2) + std::pow(disY, 2)));

            return int (dis/l);
        }();


        // some variables, that define the innercircle of the colortriangle
        QPointF pointI = [](){
            float alpha = std::atan(std::abs(yb-yg) / std::abs(xb-xg));
            float beta = std::atan(std::abs(yb-yr) / std::abs(xb-xr));
            float omega = alpha - beta;
            float slopeB = std::tan(omega/2 + beta);

            beta = std::atan(std::abs(xr-xg) / std::abs(yr-yg));
            alpha = std::atan(std::abs(yb-yr)/std::abs(xb-xr));
            omega = beta + alpha;
            float slopeR = std::tan(beta - omega/2);

            float yInterceptB = yb - xb*slopeB;
            float yInterceptR = yr - xr*slopeR;

            float x = (yInterceptB - yInterceptR) / (slopeR - slopeB);

            // y = tan(omega/2)x+b    x = (e-d)/(a-b)
            // y = ax+d     d = y - ax

            return QPointF(double(x), double(slopeB*x + yInterceptB));
        }();

        float radius = [](){
            float alpha = std::atan( std::abs(yb - float(pointI.y()) ) / std::abs(xb - float(pointI.x()) ));
            float beta = std::atan( std::abs(yb - yr) / std::abs(xb - xr));
            float omega = alpha - beta;

            float lineBR = float(std::sqrt(std::pow(std::abs(xb-xr), 2) + std::pow(std::abs(yb-yr), 2)));

            return std::tan(omega)*lineBR;
            // sin a = GK/HY  GK = sin a * HY
        }();



        // evaluate a point on cie plane into a rgb value
        QRgb cie_color_plane(float x, float y, float z){
            float r, g, b;

            float rx, ry, rz, gx, gy, gz, bx, by, bz;
            rx = (yg * zb) - (yb * zg); ry = (xb * zg) - (xg * zb); rz = (xg * yb) - (xb * yg);
            gx = (yb * zr) - (yr * zb); gy = (xr * zb) - (xb * zr);  gz = (xb * yr) - (xr * yb);
            bx = (yr * zg) - (yg * zr); by = (xg * zr) - (xr * zg);  bz = (xr * yg) - (xg * yr);

            float rw, gw, bw;
            rw = ((rx * xw) + (ry * yw) + (rz * zw)) / yw;
            gw = ((gx * xw) + (gy * yw) + (gz * zw)) / yw;
            bw = ((bx * xw) + (by * yw) + (bz * zw)) / yw;

            rx = rx / rw;  ry = ry / rw;  rz = rz / rw;
            gx = gx / gw;  gy = gy / gw;  gz = gz / gw;
            bx = bx / bw;  by = by / bw;  bz = bz / bw;

            r = (rx * x) + (ry * y) + (rz * z);
            g = (gx * x) + (gy * y) + (gz * z);
            b = (bx * x) + (by * y) + (bz * z);


            float w = - std::min(std::min(0.0f, r), std::min(g, b));
            if (w > 0){
                 r += w;  g += w; b += w;
            }

            float greatest = std::max(r, std::max(g, b));
            if (greatest > 0) {
                r /= greatest;
                g /= greatest;
                b /= greatest;
            }


        #define fast_cast(f) static_cast<int>(f * 255.0f)
            return qRgb( fast_cast(r), fast_cast(g), fast_cast(b));
        #undef fast_cast//(f)
        }
    }

    // diffrent types of coloring

    // will be deleted soon
    QRgb wavelength_color(int iterations){
        float x, y, z;
        int iter = iterations % (cie_len + times);
        if(iter < cie_len){
            float XYZ, X, Y, Z;
            X = cie_colour_match[size_t(iter % cie_len)][0];
            Y = cie_colour_match[size_t(iter % cie_len)][1];
            Z = cie_colour_match[size_t(iter % cie_len)][2];
            XYZ = (X + Y + Z);
            x = X / XYZ;
            y = Y / XYZ;
            z = Z / XYZ;
            return cie_color_plane(x, y,z);
        }
        else{
            x = calculate_x_y(cie_colour_match.back(), 0) - (iter - cie_len) * disX/ times;
            y = calculate_x_y(cie_colour_match.back(), 1) - (iter - cie_len) * disY/ times;
            z = 1 - (x + y);
            return cie_color_plane(x, y, z);
        }
    }



    QRgb cie_innercircle(int iterations){
        #define PI 3.14159265359
        float x = float(std::cos(2*PI/160 * (iterations % 160 + 1) )) * radius + float(pointI.x());
        float y = float(std::sin(2*PI/160 * (iterations % 160 + 1) )) * radius + float(pointI.y());
        #undef PI
        float z = 1 - (x + y);

        return cie_color_plane(x, y, z);
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



    QRgb colored_c_plane(std::complex<double> p){
        double r = 2.0;
        float x = float( std::cos(std::arg(p)) * std::abs(p)*double(radius)/r + pointI.x());
        float y = float( std::sin(std::arg(p)) * std::abs(p)*double(radius)/r + pointI.y());
        float z = 1 - (x + y);

        return cie_color_plane(x, y, z);
    }

    QRgb colored_by_argument(std::complex<double> p){
        float x = float(std::cos(std::arg(p)) + pointI.x());
        float y = float(std::sin(std::arg(p)) + pointI.y());
        float z = 1 - (x + y);

        return cie_color_plane(x, y, z);
    }

    QRgb black_white_distance(double dist){
        int intensity = int(255.0 * dist / 2.0);
        return qRgb(intensity,intensity,intensity);
    }

    // TODO: improve this shitty function
    QRgb colored_orbit(double dist, std::complex<double> p){
        int value = int(255.0 * dist / 2.0);
#define PI 3.14159265359
        QColor color = QColor::fromHsv(int(std::arg(p)*360.0/(2*PI)), 255, value);
#undef PI
        return qRgb(color.red(), color.green(), color.blue());
    }
}
