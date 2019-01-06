#ifndef ZEICHENFLAECHE_H
#define ZEICHENFLAECHE_H

#include <QWidget>
#include <QMouseEvent>
#include <QPixmap>
#include <QImage>
#include "renderthread.h"

namespace Ui {
class Zeichenflaeche;
}

class Zeichenflaeche : public QWidget
{
    Q_OBJECT

public:
    Zeichenflaeche(QWidget *parent= nullptr);
    ~Zeichenflaeche();

    void render();
    void render(double real, double imag, double zoom);

    void set_formula(QRgb (*funktionsZeiger) (std::complex<double>, int));
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
    QPoint newMousePos = QPoint(0, 0);
    QPointF offset = QPointF(0, 0);
    QPoint lastDragPos = QPoint(0, 0);
    QPoint pixmapOffset = QPoint(0, 0);
    double scale;
    double curScale;
};

#endif // ZEICHENFLAECHE_H
