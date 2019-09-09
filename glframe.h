///////////////////////////////////////////////
// "Piškvorky"
// Jakub Benda (c) 2009

#ifndef _GLFRAME_H_
#define _GLFRAME_H_

#include <QtGui>
#include <QtOpenGL>

#include "gamecube.h"
#include "mainwindow.h"

///
/// \class GLFrame - the geometry viewframe, main (and only one)
///                     widget of the main window
///
class GLFrame : public QGLWidget
{
    // compulsory Qt macro for moc-generation
    Q_OBJECT

    public:
        // constructor
        GLFrame(QGLFormat);

        // declare parent a friend (if only it was obvious in life, too...)
        friend class MainWindow;

    protected:
        // setup OpenGL scene
        void initializeGL();

        // change dimension of the viewport
        void resizeGL(int, int);

        // commit the painting
        void paintGL();

    private slots:
        // (no slots at the moment)

    private:
        // input event receivers
        void mouseMoveEvent(QMouseEvent*);
        void mousePressEvent(QMouseEvent*);
        void mouseReleaseEvent(QMouseEvent*);
        void wheelEvent(QWheelEvent*);

        // window background color (for use in glClearColor)
        QColor bgcolor;

        // the game plan (a set of checkable fields)
        gamecube Cube;

        // mouse cursor in window coordinates
        int X0, Y0;
};

#endif
