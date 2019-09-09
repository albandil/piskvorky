///////////////////////////////////////////////
// "Piškvorky"
// Jakub Benda (c) 2009

#ifndef _MAINWINDOW_H_
#define _MAINWINDOW_H_

#include <QtGui>
#include <QtOpenGL>

class GLFrame;

///
/// \class MainWindow - main window of the application
///
class MainWindow : public QMainWindow
{
    // compulsory Qt macro for moc-generation
    Q_OBJECT

    public:
        // default constructor
        MainWindow();

    private slots:
        // slots for menuitem signal actions
        void newgame();
        void undo();
        void spread();
        void about();

    private:
        // OpenGL viewframe
        GLFrame *glframe;
};

#endif
