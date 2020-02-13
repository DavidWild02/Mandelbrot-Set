#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QtCore>
#include <QtGui>
#include <QtWidgets/QMainWindow>

#include "mandelbrot.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    void create_actions();
    void create_menus();

    Ui::MainWindow *ui;
    QMenu* formulaMenu;
    QAction* formula0;
    QAction* formula1;
    QAction* formula2;
    QActionGroup* actionGroup;
    QMenu* coloringStyleMenu;
    QAction* coloringStyle1;
    QAction* coloringStyle2;
    QAction* coloringStyle3;
    QActionGroup* actionGroupColoring;
    QAction* resetAction;
    QActionGroup* actionGroupDrawing;
    QMenu* drawingStyleMenu;
    QAction* drawingStyle1;
    QAction* drawingStyle2;
    QAction* drawingStyle3;
    QAction* drawingStyle4;

    Mandelbrot* mandelbrot;
};

#endif // MAINWINDOW_H
