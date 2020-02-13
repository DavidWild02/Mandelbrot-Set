#include "mandelbrot.h"
#include "ui_mandelbrot.h"


std::complex<double> cJuliaMenge = std::complex<double>(0.0, 0.0);

Mandelbrot::Mandelbrot(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Mandelbrot),
    corButton(new QPushButton),
    selectButton(new QPushButton),
    zeichenflaeche_b(new Zeichenflaeche(true)),
    xEdit_b(new QLineEdit),
    yEdit_b(new QLineEdit),
    xyButton_b(new QPushButton),
    zoomEdit_b(new QLineEdit),
    zoomButton_b(new QPushButton),
    saveButton_b(new QPushButton),
    zeichenflaeche_j(new Zeichenflaeche(false)),
    xEdit_j(new QLineEdit),
    yEdit_j(new QLineEdit),
    xyButton_j(new QPushButton),
    zoomEdit_j(new QLineEdit),
    zoomButton_j(new QPushButton),
    saveButton_j(new QPushButton)
{
    ui->setupUi(this);

    // set the text, etc. for the Buttons, etc.
    QLabel* setXCoordinates_b = new QLabel("set X Coordinate:");
    setXCoordinates_b->setBuddy(xEdit_b);
    QLabel* setYCoordinates_b = new QLabel("set Y Coordinate:");
    setYCoordinates_b->setBuddy(yEdit_b);
    xyButton_b->setText("Koordinaten bestaetigen");
    QLabel* setZoom_b = new QLabel("set Zoom:");
    setZoom_b->setBuddy(zoomEdit_b);
    zoomButton_b->setText("Zoom festlegen");
    saveButton_b->setText("Save Mandelbrot-set");

    QLabel* setXCoordinates_j = new QLabel("set X Coordinate:");
    setXCoordinates_j->setBuddy(xEdit_j);
    QLabel* setYCoordinates_j = new QLabel("set Y Coordinate:");
    setYCoordinates_j->setBuddy(yEdit_j);
    xyButton_j->setText("Koordinaten bestaetigen");
    QLabel* setZoom_j = new QLabel("set Zoom:");
    setZoom_j->setBuddy(zoomEdit_j);
    zoomButton_j->setText("Zoom festlegen");
    saveButton_j->setText("Save Julia-set");

    corButton->setText(tr("Koordinaten übertragen"));
    QLabel* newCorJM = new QLabel();
    newCorJM->setText("-0.5 + 0i");
    selectButton->setText(tr("Punkt aussuchen"));


    QRegExp rExp("[+-]?\\d+\\.?\\d*");
    QValidator* floatValidator = new QRegExpValidator(rExp, this);
    xEdit_b->setValidator(floatValidator);
    yEdit_b->setValidator(floatValidator);
    xEdit_j->setValidator(floatValidator);
    yEdit_j->setValidator(floatValidator);
    QRegExp zoomRExp("([+-]?[1-9]+\\.?\\d*)|(0\\.0*[1-9]{1}\\d*)");
    QValidator* zoomValidator = new QRegExpValidator(zoomRExp, this);
    zoomEdit_b->setValidator(zoomValidator);
    zoomEdit_j->setValidator(zoomValidator);



    // this part is about how to structure the GUI
    QBoxLayout* rowCoordinates = new QBoxLayout(QBoxLayout::LeftToRight);

    rowCoordinates->addWidget(setXCoordinates_b);
    rowCoordinates->addWidget(xEdit_b);
    rowCoordinates->addWidget(setYCoordinates_b);
    rowCoordinates->addWidget(yEdit_b);
    rowCoordinates->addWidget(xyButton_b);

    rowCoordinates->addWidget(setXCoordinates_j);
    rowCoordinates->addWidget(xEdit_j);
    rowCoordinates->addWidget(setYCoordinates_j);
    rowCoordinates->addWidget(yEdit_j);
    rowCoordinates->addWidget(xyButton_j);


    QBoxLayout* rowZoom = new QBoxLayout(QBoxLayout::LeftToRight);

    rowZoom->addWidget(setZoom_b);
    rowZoom->addWidget(zoomEdit_b);
    rowZoom->addWidget(zoomButton_b);

    rowZoom->addWidget(setZoom_j);
    rowZoom->addWidget(zoomEdit_j);
    rowZoom->addWidget(zoomButton_j);


    QGridLayout* alles = new QGridLayout(this);
    alles->addWidget(zeichenflaeche_b, 0, 0, 5, 2);
    alles->addWidget(zeichenflaeche_j, 0, 2, 5, 2);
    alles->addLayout(rowCoordinates, 5, 0, 1, 4, Qt::AlignLeft);
    alles->addLayout(rowZoom, 6, 0, 1, 4, Qt::AlignLeft);
    alles->addWidget(newCorJM, 7, 0, 1, 1, Qt::AlignHCenter);
    alles->addWidget(selectButton, 7, 1, 1, 1);
    alles->addWidget(corButton, 7, 2, 1, 2);
    alles->addWidget(saveButton_b, 8, 0, 1, 2);
    alles->addWidget(saveButton_j, 8, 2, 1, 2);
    setLayout(alles);


    // here are the signals and slots
    auto f = [this](){
        zeichenflaeche_b->render(xEdit_b->text().toDouble(), yEdit_b->text().toDouble(), 1/zoomEdit_b->text().toDouble());
    };
    connect(xyButton_b, &QPushButton::clicked, zeichenflaeche_b, f);
    connect(zoomButton_b, &QPushButton::clicked, zeichenflaeche_b, f);

    connect(xEdit_b, SIGNAL(returnPressed()), xyButton_b, SLOT(click()));
    connect(yEdit_b, SIGNAL(returnPressed()), xyButton_b, SLOT(click()));
    connect(zoomEdit_b, SIGNAL(returnPressed()), zoomButton_b, SLOT(click()));

    connect(zeichenflaeche_b, &Zeichenflaeche::updated_values, this, [this](double xcor, double ycor, double zoom){
       xEdit_b->setText(QString::number(xcor));
       yEdit_b->setText(QString::number(ycor));
       zoomEdit_b->setText(QString::number(1/zoom));
    });
    connect(saveButton_b, &QPushButton::clicked, this, [this](){
       QString fileName = QFileDialog::getSaveFileName(this, tr("Save File"), "C:/Users/User/Pictures/untitled_mandelbrot_set.jpg", tr("Image (*.png, *.jpg, *.xpm)"));
       zeichenflaeche_b->get_pixmap().save(fileName);
    });


    auto g = [this](){
        zeichenflaeche_j->render(xEdit_j->text().toDouble(), yEdit_j->text().toDouble(), 1/zoomEdit_j->text().toDouble());
    };
    connect(xyButton_j, &QPushButton::clicked, zeichenflaeche_j, g);
    connect(zoomButton_j, &QPushButton::clicked, zeichenflaeche_j, g);

    connect(xEdit_j, SIGNAL(returnPressed()), xyButton_j, SLOT(click()));
    connect(yEdit_j, SIGNAL(returnPressed()), xyButton_j, SLOT(click()));
    connect(zoomEdit_j, SIGNAL(returnPressed()), zoomButton_j, SLOT(click()));

    connect(zeichenflaeche_j, &Zeichenflaeche::updated_values, this, [this](double xcor, double ycor, double zoom){
       xEdit_j->setText(QString::number(xcor));
       yEdit_j->setText(QString::number(ycor));
       zoomEdit_j->setText(QString::number(1/zoom));
    });


    connect(selectButton, &QPushButton::clicked, this, [this](){
        zeichenflaeche_b->selectPoint = true;
    });
    connect(zeichenflaeche_b, &Zeichenflaeche::selected_point, this, [=](QPointF p){
        newCorJM->setText(QString::number(p.x()) + " + "+ QString::number(p.y()) + "i");
    });
    connect(corButton, &QPushButton::clicked, this, [=](){
        QStringList qsList =  newCorJM->text().split(" + ");
        double real = qsList[0].toDouble();
        double imag = qsList[1].remove(qsList[1].size() - 1, 1).toDouble();
        cJuliaMenge = std::complex<double>(real, imag);
        zeichenflaeche_j->render();
    });
    connect(saveButton_j, &QPushButton::clicked, this, [this](){
       QString fileName = QFileDialog::getSaveFileName(this, tr("Save File"), "C:/Users/User/Pictures/untitled_julia_set.jpg", tr("Image (*.png, *.jpg, *.xpm)"));
       zeichenflaeche_j->get_pixmap().save(fileName);
    });


    // default settings
    RenderThread::formula = [](std::complex<double> c, std::complex<double> z){
        return z*z + c;
    };
    RenderThread::colorStyle = coloring::colorfunction::wavelength;
    RenderThread::drawingStyle = coloring::drawingfunction::iteration;
    reset_settings();
}

Mandelbrot::~Mandelbrot()
{
    delete ui;
}

void Mandelbrot::reset_settings(){
    xEdit_b->setText(QString::number(-0.5));
    yEdit_b->setText(QString::number(0));
    zoomEdit_b->setText(QString::number(1));
    zeichenflaeche_b->render(xEdit_b->text().toDouble(), yEdit_b->text().toDouble(), 1/zoomEdit_b->text().toDouble());

    corButton->clicked();
    xEdit_j->setText(QString::number(0));
    yEdit_j->setText(QString::number(0));
    zoomEdit_j->setText(QString::number(1));
    zeichenflaeche_j->render(xEdit_j->text().toDouble(), yEdit_j->text().toDouble(), 1/zoomEdit_j->text().toDouble());
}
