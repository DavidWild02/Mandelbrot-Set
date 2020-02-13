#ifndef RENDERTHREAD_H
#define RENDERTHREAD_H

#include <QObject>
#include <QThread>
#include <QtCore>
#include <complex>

#include "coloring.h"


const QSizeF MINSIZE = QSizeF(450, 300);
extern std::complex<double> cJuliaMenge;



// The Renderthread, does the actually work. It calculates the mandelbrot or Julia Set and then give it to zeichenflaeche
class RenderThread : public QThread
{
    Q_OBJECT
public:
    explicit RenderThread(QObject *parent = nullptr);
    ~RenderThread() override;
    QPointF calculate_coordinates(QSize size, double x, double y, double scale);

    void run() override;

#define COMPLEX std::complex<double>
    static std::function<COMPLEX(COMPLEX, COMPLEX)> formula;
#undef COMPLEX
    static coloring::colorfunction colorStyle;
    static coloring::drawingfunction drawingStyle;
    QRgb in_mandelbrot_set(std::complex<double> complexPoint, int maxIterations);

    bool isForMandelbrot;

    RenderThread(const RenderThread&) = default;
    RenderThread& operator=(const RenderThread&) = default;

signals:
    void result_ready(QImage qImage, double scaleFactor);

public slots:
    void do_work(const QSize size, const double xcor, const double ycor, double zoom);

private:
    const QSizeF* VIEW = new QSizeF(3.0, 2.0);
    QMutex mutex;
    QWaitCondition condition;
    QSize size;
    double xcor;
    double ycor;
    double zoom;
    bool restart;
    bool abort;
};

#endif // RENDERTHREAD_H
