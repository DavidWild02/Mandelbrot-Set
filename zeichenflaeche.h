#ifndef ZEICHENFLAECHE_H
#define ZEICHENFLAECHE_H

#include <QWidget>
#include <QMouseEvent>
#include <QPixmap>
#include <QImage>
#include <QPainter>
#include "renderthread.h"

namespace Ui {
class Zeichenflaeche;
}



// Zeichenflaeche is a class for interacting with the image, zooming, moving, etc. It doesnt calculate the mandelbrot set,
// because, that would freeze the UI. The Renderthread, does the calculation seperatly.
class Zeichenflaeche : public QWidget
{
    Q_OBJECT

public:
    Zeichenflaeche(bool isFMB, QWidget *parent= nullptr);
    ~Zeichenflaeche();

    void render();
    void render(double real, double imag, double zoom);

    QPixmap get_pixmap();
    bool selectPoint = false;


protected:
    void resizeEvent(QResizeEvent*);
    void paintEvent(QPaintEvent*);
    void mousePressEvent(QMouseEvent* event);
    void mouseReleaseEvent(QMouseEvent* event);
    void mouseMoveEvent(QMouseEvent* event);
    void wheelEvent(QWheelEvent* event);

signals:
    void updated_values(double real, double imag, double zoom);
    void selected_point(QPointF selectedPoint);

private:
    Ui::Zeichenflaeche *ui;

    QPixmap pixmap;
    RenderThread renderthread;
    QPoint newCenterPos = QPoint(0, 0);
    QPointF offset = QPointF(0, 0);
    QPoint lastDragPos = QPoint(0, 0);
    QPoint pixmapOffset = QPoint(0, 0);
    double scale;
    double curScale;
};

#endif // ZEICHENFLAECHE_H
