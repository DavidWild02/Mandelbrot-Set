#ifndef MANDELBROT_H
#define MANDELBROT_H

#include <QtWidgets>
#include <QWidget>
#include <QGridLayout>
#include <QLabel>
#include <QPushButton>
#include <QLineEdit>
#include <QRegExpValidator>

#include "zeichenflaeche.h"
#include "renderthread.h"


namespace Ui {
class Mandelbrot;
}

class Mandelbrot : public QWidget
{
    Q_OBJECT
public:
    explicit Mandelbrot(QWidget *parent = 0);
    ~Mandelbrot();

private:
    Ui::Mandelbrot *ui;
    QPushButton* corButton;
    QPushButton* selectButton;

    Zeichenflaeche* zeichenflaeche_b;
    QLineEdit* xEdit_b;
    QLineEdit* yEdit_b;
    QPushButton* xyButton_b;
    QLineEdit* zoomEdit_b;
    QPushButton* zoomButton_b;

    Zeichenflaeche* zeichenflaeche_j;
    QLineEdit* xEdit_j;
    QLineEdit* yEdit_j;
    QPushButton* xyButton_j;
    QLineEdit* zoomEdit_j;
    QPushButton* zoomButton_j;
};

#endif // MANDELBROT_H
