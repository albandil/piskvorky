///////////////////////////////////////////////
// "Piškvorky"
// Jakub Benda (c) 2009

#include <QtGui>

#include "mainwindow.h"
#include "glframe.h"

MainWindow::MainWindow()
{
    // create OpenGL widget with alpha support
    QGLFormat glformat;
    glformat.setAlpha(true);
    glframe = new GLFrame(glformat);
    setCentralWidget(glframe);

    // create menubar
    QMenu *hra, *napoveda;
    QAction *nova, *zpet, *roztahnout, *konec, *oprogramu, *oqt;

    nova = new QAction(tr("&Nová"), this);
    nova->setShortcut(tr("F2"));
    nova->setStatusTip(tr("Začne novou hru"));
    connect(nova, SIGNAL(triggered()), this, SLOT(newgame()));

    zpet = new QAction(tr("O tah zpět"), this);
    zpet->setShortcut(tr("Backspace"));
    zpet->setStatusTip(tr("Vrátí zpět poslední tah."));
    connect(zpet, SIGNAL(triggered()), this, SLOT(undo()));

    roztahnout = new QAction(tr("Roztáhnout"), this);
    roztahnout->setShortcut(tr("Space"));
    roztahnout->setStatusTip(tr("Roztáhne od sebe malé krychličky pro snadnější dostupnost."));
    roztahnout->setCheckable(true);
    connect(roztahnout, SIGNAL(triggered()), this, SLOT(spread()));

    konec = new QAction(tr("&Konec"), this);
    konec->setShortcut(tr("Ctrl+Q"));
    konec->setStatusTip(tr("Ukončí program"));
    connect(konec, SIGNAL(triggered()), qApp, SLOT(quit()));

    oprogramu = new QAction(tr("Ná&pověda"), this);
    oprogramu->setShortcut(tr("F1"));
    oprogramu->setStatusTip(tr("Zobrazí dialog s informacemi o aplikaci."));
    connect(oprogramu, SIGNAL(triggered()), this, SLOT(about()));

    oqt = new QAction(tr("O knihovně Qt"), this);
    oqt->setStatusTip(tr("Zobrazí dialog s informacemi o použité verzi knihovny Qt."));
    connect(oqt, SIGNAL(triggered()), qApp, SLOT(aboutQt()));

    hra = menuBar()->addMenu(tr("&Hra"));
    hra->addAction(nova);
    hra->addSeparator();
    hra->addAction(zpet);
    hra->addAction(roztahnout);
    hra->addSeparator();
    hra->addAction(konec);

    napoveda = menuBar()->addMenu(tr("Ná&pověda"));
    napoveda->addAction(oprogramu);
    napoveda->addAction(oqt);

    // setup other parameters of the window
    setWindowTitle(tr("Piškvorky"));
    resize(640, 480);
}

void MainWindow::newgame()
{
    // create new game and redraw
    glframe->Cube.reset();
    glframe->updateGL();
}

void MainWindow::undo()
{
    // step back
    glframe->Cube.undo();
    glframe->updateGL();
}

void MainWindow::spread()
{
    glframe->Cube.switchDisperse();
    glframe->updateGL();
}

void MainWindow::about()
{
    // apllication "About" box
    QMessageBox::information(this, tr("Nápověda k programu Piškvorky"),
        tr("Tažením lze hrací pole otáčet, pravým tlačítkem "
        "se označuje vybrané pole pro hráče, který je na tahu (automaticky se střídají). Všechny "
        "tahy dané hry lze vrátit zpět klávesou Backspace. Mezerníkem se malé krychličky oddalují/"
        "přibližují, aby bylo možné i označovat vnitřní pole, i vidět celou velkou krychli jako celistvý,"
        " přehledný objekt. Na zoomování se hodí kolečko myši.\n"
        "     Hodně zábavy přeje Dil (albandil@atlas.cz)."));
}
