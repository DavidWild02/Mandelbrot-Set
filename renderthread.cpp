#include "renderthread.h"

// default settings
coloring::colorfunction RenderThread::colorStyle = coloring::colorfunction::zickZackRGB;
coloring::drawingfunction RenderThread::drawingStyle = coloring::drawingfunction::iteration;
#define COMPLEX std::complex<double>
std::function<COMPLEX(COMPLEX, COMPLEX)> RenderThread::formula = [](COMPLEX c, COMPLEX z){
    return z * z + c;
};
#undef COMPLEX

RenderThread::RenderThread(QObject *parent)
    : QThread(parent)
{
    restart = false;
    abort = false;
}

RenderThread::~RenderThread(){
    mutex.lock();
    condition.wakeOne();
    abort = true;
    mutex.unlock();

    wait();
}

// Zeichenflaeche need sometimes some coordinates for zooming and scaling
QPointF RenderThread::calculate_coordinates(QSize size, double x, double y, double scale){
    double proportionH = MINSIZE.height()/size.height();
    double proportionW = MINSIZE.width()/size.width();
    double zoom = ( proportionH >  proportionW ) ? proportionH :  proportionW;
    return QPointF(x/MINSIZE.width()*VIEW->width()*scale*zoom,
                   y/MINSIZE.height()*VIEW->height()*scale*zoom);
}

// doesnt calculate, does only control the thread in the background
void RenderThread::do_work(const QSize size, const double xcor, const double ycor, double zoom)
{
    QMutexLocker locker(&mutex);

    this->size = size;
    this->xcor = xcor;
    this->ycor = ycor;
    this->zoom = zoom;

    if(!isRunning())
        start(LowPriority);
    else{
        condition.wakeOne();
        restart = true;
    }
}

// automatically called by start() and condition.wakeOne()
void RenderThread::run(){
    forever{
        mutex.lock();
        QSize size = this->size;
        double xcor = this->xcor;
        double ycor = this->ycor;
        double zoom = this->zoom;
        mutex.unlock();


        QImage qImage(size.width(), size.height(), QImage::Format_RGB32);

        int i = 0;
        while(i <= 8){
            int maxIterations = (1 << (i * 2 + 6)) + 36;
            bool allBlack = true;


            // x|y (window) -> x+yi (complex plane)
            // calculates only the coordinates from the window, image-size into scaled coordinates of the complex plane.
            for(int x = 0; x < size.width(); x++)
            {
                if(restart) break;
                double real = x - 1.0/2.0*size.width();

                for(int y = 0; y < size.height(); y++)
                {
                    if(restart) break;
                    if(abort) return;

                    double imag = y - 1.0/2.0*size.height();

                    QPointF c = calculate_coordinates(size, real, imag, zoom);


                    // choose color and draw point
                    qImage.setPixel(x, y, in_mandelbrot_set(std::complex<double>(c.x() + xcor, c.y() - ycor), maxIterations));

                    if(qImage.pixelColor(x, y) != QColor(0, 0, 0))
                        allBlack = false;
                }
            }

            if(allBlack && i == 0)
                i = 3;
            else if(!restart)
                emit result_ready(qImage, zoom);

            ++i;
        }

        mutex.lock();
        if(!restart)
            condition.wait(&mutex);
        restart = false;
        mutex.unlock();
    }
}



QRgb RenderThread::in_mandelbrot_set(std::complex<double> complexPoint, int maxIterations){
    QMutex qmutex;

    std::complex<double> z = (isForMandelbrot) ? 0 : complexPoint;
    std::complex<double> c = (isForMandelbrot) ? complexPoint : cJuliaMenge;


    switch(drawingStyle){
        case coloring::drawingfunction::iteration :
            for(int i = 0; i < maxIterations; i++)
            {
                z = formula(c, z);

                // point is not part of the Mandelbrotset = colored,
                // how to colorate the point is pointless, you can be creative
                if(std::abs(z) > 2){
                    switch(colorStyle){
                        case coloring::colorfunction::wavelength :
                            return coloring::wavelength_color(i);
                        case coloring::colorfunction::cieInnercircle :
                            return coloring::cie_innercircle(i);
                        case coloring::colorfunction::zickZackRGB :
                            return coloring::zick_zack_rgb(i);
                    }
                }
            }
            // point is a element of the Mandelbrotset = black
            return qRgb(0,0,0);

        case coloring::drawingfunction::innerColoring :
            for(int i = 0; i < maxIterations; i++)
            {
                z = formula(c,z);
                if(std::abs(z) > 2)
                    return qRgb(0,0,0);
            }
            return coloring::colored_c_plane(z);

        case coloring::drawingfunction::coloredArgument :
            for(int i = 0; i < maxIterations; i++)
            {
                z = formula(c,z);
                if(std::abs(z) > 2)
                    return coloring::colored_by_argument(z);
            }
            return qRgb(0,0,0);

        case coloring::drawingfunction::orbitTrapping :
            double dist = 1e10;
            for(int i = 0; i < maxIterations; i++)
            {
                z = formula(c,z);
                if(std::abs(z) > 2)
                    return coloring::black_white_distance(std::sqrt(dist));
                dist = std::min(dist, std::pow(std::abs(z - std::complex<double>(0,0)), 2));
            }
            return qRgb(255,255,255);
    }
}
