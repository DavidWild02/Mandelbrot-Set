#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    setGeometry(300 ,300 ,600 ,400 );
    Mandelbrot* widget = new Mandelbrot(this);
    setCentralWidget(widget);
}

MainWindow::~MainWindow()
{
    delete ui;
}
