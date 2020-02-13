#include "zeichenflaeche.h"
#include "ui_zeichenflaeche.h"


const double scaleFactor = 0.8;


Zeichenflaeche::Zeichenflaeche(bool isFMB, QWidget *parent)  :
    QWidget(parent),
    ui(new Ui::Zeichenflaeche),
    renderthread(new RenderThread())
{
    ui->setupUi(this);
    curScale = scale;

    renderthread.isForMandelbrot = isFMB;

    // gets the image from the RenderThread and update the widget
    connect(&renderthread, &RenderThread::result_ready, this, [this](QImage image, double cScale){
        pixmap = QPixmap::fromImage(image);
        curScale = cScale;
        newCenterPos = QPoint(0, 0);
        update();
    });

#ifndef QT_NO_CURSOR
    setCursor(Qt::CrossCursor);
#endif
}

Zeichenflaeche::~Zeichenflaeche()
{
    delete ui;
}

void Zeichenflaeche::render(){
    emit updated_values(offset.x(), offset.y(), scale);
    renderthread.do_work(this->size(), offset.x(), offset.y(), scale);
}

void Zeichenflaeche::render(double real, double imag, double zoom){
    offset = QPointF(real, imag);
    scale = zoom;
    render();
}

QPixmap Zeichenflaeche::get_pixmap(){
    return pixmap;
}

void Zeichenflaeche::paintEvent(QPaintEvent*){
    QPainter painter(this);

    if(curScale != scale) {
        double f = curScale/scale;
        pixmap = pixmap.scaled(int(this->width()*f), int(this->height()*f));
    }
    painter.drawPixmap(pixmapOffset + newCenterPos, pixmap);
}

void Zeichenflaeche::resizeEvent(QResizeEvent*){
    render();
}

void Zeichenflaeche::mousePressEvent(QMouseEvent *event){
    if (event->button() == Qt::LeftButton)
    {
        if(selectPoint){
            int xMouse = event->x() - this->width()/2;
            int yMouse = this->height()/2 - event->y();
            emit selected_point(renderthread.calculate_coordinates(this->size(), xMouse, yMouse, scale) + offset);
        }
        else{
            lastDragPos = event->pos();
        }
    }
}

void Zeichenflaeche::mouseMoveEvent(QMouseEvent *event){
    if (event->buttons() & Qt::LeftButton && !selectPoint){
        pixmapOffset = event->pos() - lastDragPos;
        update();
    }
}

void Zeichenflaeche::mouseReleaseEvent(QMouseEvent *event){
    if(event->button() == Qt::LeftButton){
        if(selectPoint)
            selectPoint = false;
        else{
            offset += renderthread.calculate_coordinates(this->size(), -pixmapOffset.x(), pixmapOffset.y(), scale);
            pixmapOffset = QPoint(0, 0);
            lastDragPos = QPoint(0, 0);
            render();
        }
    }
}

void Zeichenflaeche::wheelEvent(QWheelEvent *event){
    int numSteps = event->delta() / 8 / 15;

    int xMouse = event->x() - this->width()/2;
    int yMouse = this->height()/2 - event->y();
    offset += renderthread.calculate_coordinates(this->size(), xMouse, yMouse, scale) * (1.0 - pow(scaleFactor, numSteps));
    scale *= pow(scaleFactor, numSteps);
    QPoint sizePoint = QPoint(this->width(), this->height());
    newCenterPos = -sizePoint * (curScale/scale - 1.0 )/2.0;
    newCenterPos += QPoint(-xMouse, yMouse) * (curScale/scale - 1.0);
    update();
    render();
}


