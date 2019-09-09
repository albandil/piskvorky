///////////////////////////////////////////////
// "Piškvorky"
// Jakub Benda (c) 2009

#include <QtGui>

#include "mainwindow.h"
#include "glframe.h"

int main(int argc, char* argv[])
{
    // run built-in argument processing
    QApplication app(argc,argv);

    // set international en-/decoding
    QTextCodec::setCodecForLocale(QTextCodec::codecForName("UTF-8"));

    // create and display the main window
    MainWindow mainwindow;
    mainwindow.showMaximized();

    // execute the program
    return app.exec();
}
