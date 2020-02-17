#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    mandelbrot(new Mandelbrot(this))
{
    ui->setupUi(this);

    setGeometry(300 ,300 ,600 ,400 );
    setCentralWidget(mandelbrot);

    create_actions();
    create_menus();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::create_actions(){
    actionGroup = new QActionGroup(this);
    actionGroupColoring = new QActionGroup(this);
    actionGroupDrawing = new QActionGroup(this);

#ifndef NEW_FORMULA
#define NEW_FORMULA(NAME, FORMULA)                                                              \
     NAME = new QAction(tr("z = " #FORMULA), this);                                             \
    connect(NAME, &QAction::triggered, this, [](){                                          \
        RenderThread::formula = [](std::complex<double> c, std::complex<double> z){          \
            return FORMULA;                                                                     \
        };                                                                                     \
    });                                                                                         \
    actionGroup->addAction(NAME);
#endif

#ifndef NEW_COLORINGSTYLE
#define NEW_COLORINGSTYLE(NAME, STYLE)                                   \
    NAME = new QAction(tr(#STYLE), this);                               \
    connect(NAME, &QAction::triggered, this, [](){                  \
        RenderThread::colorStyle = coloring::colorfunction::STYLE;    \
    });                                                              \
    actionGroupColoring->addAction(NAME);
#endif

#ifndef NEW_DRAWINGSTYLE
#define NEW_DRAWINGSTYLE(NAME, STYLE)          \
    NAME = new QAction(tr(#STYLE), this);     \
    connect(NAME, &QAction::triggered, this, [](){   \
        RenderThread::drawingStyle = coloring::drawingfunction::STYLE;   \
    });   \
    actionGroupDrawing->addAction(NAME);
#endif

    NEW_FORMULA(formula0, z+c);
    NEW_FORMULA(formula1, z*z+c);
    NEW_FORMULA(formula2, z*z*z+c);

    NEW_COLORINGSTYLE(coloringStyle1, zickZackRGB);
    NEW_COLORINGSTYLE(coloringStyle2, circleHSV);

    NEW_DRAWINGSTYLE(drawingStyle1, iteration);
    NEW_DRAWINGSTYLE(drawingStyle2, innerColoring);
    NEW_DRAWINGSTYLE(drawingStyle3, coloredArgument);
    NEW_DRAWINGSTYLE(drawingStyle4, orbitTrapping);

    /*
    connect(resetAction, &QAction::triggered, this, [this](){
        mandelbrot->reset_settings();
    });
    // Error here
    */

#undef NEW_FORMULA
#undef NEW_DRAWING_STYLE
#undef NEW_COLORING_STYLE
    formula1->setChecked(true);
    drawingStyle1->setChecked(true);
}

void MainWindow::create_menus(){
    formulaMenu = menuBar()->addMenu(tr("choose formula"));
    formulaMenu->addAction(formula0);
    formulaMenu->addAction(formula1);
    formulaMenu->addAction(formula2);
    coloringStyleMenu = menuBar()->addMenu(tr("choose coloringstyle"));
    coloringStyleMenu->addAction(coloringStyle1);
    coloringStyleMenu->addAction(coloringStyle2);
    drawingStyleMenu = menuBar()->addMenu(tr("choose drawingstyle"));
    drawingStyleMenu->addAction(drawingStyle1);
    drawingStyleMenu->addAction(drawingStyle2);
    drawingStyleMenu->addAction(drawingStyle3);
    drawingStyleMenu->addAction(drawingStyle4);
    resetAction = menuBar()->addAction(tr("reset"));
}
