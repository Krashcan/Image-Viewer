
#include "imageviewer.h"
#include "ui_imageviewer.h"

ImageViewer::ImageViewer(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::ImageViewer)
{
    ui->setupUi(this);

    imageLabel= new QLabel;
    imageLabel->setBackgroundRole(QPalette::Base);
    imageLabel->setSizePolicy(QSizePolicy::Ignored,QSizePolicy::Ignored);
    imageLabel->setScaledContents(true);

    scrollArea= new QScrollArea;
    scrollArea->setBackgroundRole(QPalette::Dark);
    scrollArea->setWidget(imageLabel);
    setCentralWidget(scrollArea);

    setWindowTitle(tr("Image Viewer"));

    resize(500,400);

    openAct= new QAction(tr("&Open..."),this);
    openAct->setShortcut(tr("Ctrl+O"));
    printAct= new QAction(tr("&Print..."), this);
    printAct->setShortcut(tr("Ctrl+P"));
    printAct->setEnabled(false);

    exitAct= new QAction(tr("E&xit"), this);
    exitAct->setShortcut(tr("Ctrl+Q"));

    zoomInAct= new QAction(tr("Zoom &In (25%)"), this);
    zoomInAct->setShortcut(tr("Ctrl+="));
    zoomInAct->setEnabled(false);

    zoomOutAct= new QAction(tr("Zoom &Out (25%)"), this);
    zoomOutAct->setShortcut(tr("Ctrl+-"));
    zoomOutAct->setEnabled(false);

    normalSizeAct= new QAction(tr("&Normal Size"), this);
    normalSizeAct->setShortcut(tr("Ctrl+S"));
    normalSizeAct->setEnabled(false);

    fitToWindowAct= new QAction(tr("&Fit to Window"), this);
    fitToWindowAct->setEnabled("false");
    fitToWindowAct->setCheckable("true");
    fitToWindowAct->setShortcut(tr("Ctrl+F"));

    aboutAct = new QAction(tr("&About"), this);

    aboutQtAct= new QAction(tr("About &Qt"),this);

    fileMenu=new QMenu(tr("&File"), this);
    fileMenu->addAction(openAct);
    fileMenu->addAction(printAct);
    fileMenu->addAction(exitAct);

    viewMenu= new QMenu(tr("&View"),this);
    viewMenu->addAction(zoomInAct);
    viewMenu->addAction(zoomOutAct);
    viewMenu->addAction(normalSizeAct);
    viewMenu->addAction(fitToWindowAct);

    helpMenu = new QMenu(tr("&Help"),this);
    helpMenu->addAction(aboutAct);
    helpMenu->addAction(aboutQtAct);

    menuBar()->addMenu(fileMenu);
    menuBar()->addMenu(viewMenu);
    menuBar()->addMenu(helpMenu);

    connect(openAct, SIGNAL(triggered()),this ,SLOT(open()));
    connect(printAct, SIGNAL(triggered()), this, SLOT(print()));
    connect(exitAct, SIGNAL(triggered()), this, SLOT(close()));
    connect(zoomInAct, SIGNAL(triggered()), this, SLOT(zoomIn()));
    connect(zoomOutAct, SIGNAL(triggered()), this, SLOT(zoomOut()));
    connect(normalSizeAct, SIGNAL(triggered()), this, SLOT(normalSize()));
    connect(fitToWindowAct, SIGNAL(triggered()), this, SLOT(fitToWindow()));
    connect(aboutAct, SIGNAL(triggered()), this, SLOT(about()));
    connect(aboutQtAct, SIGNAL(triggered()), qApp, SLOT(aboutQt()));
}

ImageViewer::~ImageViewer()
{
    delete ui;
}
void ImageViewer::open()
{
    qDebug("open()");

    QString fileName = QFileDialog::getOpenFileName(this,
                                                    tr("Open File"),QDir::currentPath());
    if(!fileName.isEmpty()){
        QImage image(fileName);
                 if (image.isNull()) {
                     QMessageBox::information(this, tr("Image Viewer"),
                                              tr("Cannot load %1.").arg(fileName));
                     return;
        }

                 imageLabel->setPixmap(QPixmap::fromImage(image));
                          scaleFactor = 1.0;

                          printAct->setEnabled(true);
                          fitToWindowAct->setEnabled(true);
                          updateActions();

                          if (!fitToWindowAct->isChecked())
                              imageLabel->adjustSize();
    }
}
void ImageViewer::zoomIn()
{
    scaleImage(1.25);
}

void ImageViewer::zoomOut()
{
    scaleImage(0.8);
}

void ImageViewer::normalSize()
{
    imageLabel->adjustSize();
    scaleFactor = 1.0;
}
void ImageViewer::fitToWindow()
{
    bool fitToWindow = fitToWindowAct->isChecked();
    scrollArea->setWidgetResizable(fitToWindow);
    if (!fitToWindow) {
        normalSize();
    }
    updateActions();
}
void ImageViewer::updateActions()
{
    zoomInAct->setEnabled(!fitToWindowAct->isChecked());
    zoomOutAct->setEnabled(!fitToWindowAct->isChecked());
    normalSizeAct->setEnabled(!fitToWindowAct->isChecked());
}
void ImageViewer::scaleImage(double factor)
{
    Q_ASSERT(imageLabel->pixmap());
    scaleFactor *= factor;
    imageLabel->resize(scaleFactor * imageLabel->pixmap()->size());

    adjustScrollBar(scrollArea->horizontalScrollBar(), factor);
    adjustScrollBar(scrollArea->verticalScrollBar(), factor);

    zoomInAct->setEnabled(scaleFactor < 3.0);
    zoomOutAct->setEnabled(scaleFactor > 0.333);
}
void ImageViewer::adjustScrollBar(QScrollBar *scrollBar, double factor)
{
    scrollBar->setValue(int(factor * scrollBar->value()
                            + ((factor - 1) * scrollBar->pageStep()/2)));
}
