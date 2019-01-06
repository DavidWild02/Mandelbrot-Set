#include "renderthread.h"


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

QPointF RenderThread::calculate_coordinates(QSize size, double x, double y, double scale){
    double proportionH = MINSIZE.height()/size.height();
    double proportionW = MINSIZE.width()/size.width();
    double zoom = ( proportionH >  proportionW ) ? proportionH :  proportionW;
    return QPointF(x/MINSIZE.width()*VIEW->width()*scale*zoom,
                   y/MINSIZE.height()*VIEW->height()*scale*zoom);
}

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
        // Punkte des Coordinatensystems des Fensters durch gehen, und diese auf die Coordinaten der Complexen Ebene umrechnen
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
                    // Farbe auswählen und punkt einzeichnen
                    qImage.setPixel(x, y, formula(std::complex<double>(c.x() + xcor, c.y() - ycor), maxIterations));

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

// hier wird berechnet ob ein Punkt in der MandelbrotMenge liegt oder nicht
// als Rückgabewert wird eine Farbe mitgegeben
QRgb RenderThread::in_mandelbrot_set(std::complex<double> c,std::complex<double> z, int maxIterations){
    QMutex qmutex;
    QMutexLocker locker(&qmutex);
    // theoretisch hätte MAX_ITERATIONS den Wert unendlich, allerdings kann der Computer damit nicht rechnen.
    // somit muss ein endliche Zahl an Iterationen durchgegangen werden.
    // Je mehr Iterationen desto genauer wird das Ergebnis
    for(int i = 0; i < maxIterations; i++)
    {
        z = z * z + c;

        // Wenn der Punkt außerhalb der Mandelbrotmenge liegt wird er eingefärbt,
        // je nachdem wie viel Iterationen gebraucht wurden
        if(std::abs(z) > 2)
        {
            const int numOfColors = 3 * 10;
            const int numOfSteps = 10;

            int r, g, b;
            int range = 255*(i%numOfSteps + 1)/(numOfSteps);
            switch( (i % numOfColors) / numOfSteps)
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
            return qRgb(r,g,b);
        }
    }

    // liegt der Punkt innerhalb der MandelbrotMenge wird er Schwarz angemalt
    return qRgb(0,0,0);
}
