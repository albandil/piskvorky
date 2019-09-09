///////////////////////////////////////////////
// "Piškvorky"
// Jakub Benda (c) 2009

#include <QtGui>
#include <QtOpenGL>

#include <GL/glu.h>
#include <unistd.h>

#include <cmath>

#include "camera.h"
#include "glframe.h"
#include "utils.h"

camera Cam;    // the main (and the only) camera

GLFrame::GLFrame(QGLFormat format) : QGLWidget(format), Cube(4)
{
    // setup camera
    Cam.R     = 7;
    Cam.theta = 0.85;
    Cam.phi   = 0.65;
    Cam.x = 0.0;
    Cam.y = 0.0;
    Cam.z = 0.0;

    // enable mouse tracking so that we can receive mousemove events
    setMouseTracking(true);

    // reset mouse coords
    X0 = Y0 = -1;

    // sort planes
    Cube.updateVisibility();

    // store window bgcolor for use in glClearColor
    bgcolor = this->palette().color(QPalette::Window);

    // check if this colour is at use for picking
    Cube.reserveColour(bgcolor);
}

void GLFrame::initializeGL()
{
    if (format().alpha() != true)
        qFatal("No alpha buffer support!");

    // all we need here is alpha-blending
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    glClearColor(bgcolor.redF(), bgcolor.greenF(), bgcolor.blueF(), 1.0);
}

void GLFrame::resizeGL(int width, int height)
{
    glViewport(0, 0, width, height);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(72.0, (double)width/(double)height, 0.1, 1000.0);
}

void GLFrame::paintGL()
{
    //clears the buffer
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    //applies a look-at transformation
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    gluLookAt(
        /*eye*/ Cam.x + Cam.R * sin(Cam.theta) * sin(Cam.phi),
                Cam.y + Cam.R * cos(Cam.theta),
                Cam.z + Cam.R * sin(Cam.theta) * cos(Cam.phi),
        /*target*/ Cam.x, Cam.y, Cam.z,
        /*up*/ 0.0, 1.0, 0.0
    );

    // draw the game plan
    Cube.draw();

    // forces the process to finish
    glFlush();

    // microsleeps for a while to cool down the GPU
    usleep(50000);
}

void GLFrame::mouseMoveEvent(QMouseEvent* event)
{
    // rotate the model and redraw
    if (event->buttons() & Qt::LeftButton)
    {
        if (X0 > 0 && Y0 > 0)
        {
            Cam.theta += (Y0 - event->y()) * 0.005;
            if (Cam.theta > 3.0)
                Cam.theta = 3.0;
            if (Cam.theta < 0.15)
                Cam.theta = 0.15;
            Cam.phi += (X0 - event->x()) * 0.005;
            Cube.updateVisibility();
            updateGL();
        }
        X0 = event->x();
        Y0 = event->y();
    }
    else
    {
        if (Cube.cursorPick(event->x(), event->y()))
            updateGL();
    }
}

void GLFrame::mousePressEvent(QMouseEvent* event)
{
    // store current mouse coordinates
    if (event->buttons() & Qt::LeftButton)
    {
        X0 = event->x();
        Y0 = event->y();
    }

    // select chosen cube
    if (event->buttons() & Qt::RightButton)
    {
        if (Cube.selectPicked())
        {
            updateGL();
            tsign win = Cube.checkWinner();
            QString winner, loser;
            if (win == cross && !Cube.halted())
            {
                winner = tr("Červený");
                loser = tr("modrý");
            }
            if (win == circle && !Cube.halted())
            {
                winner = tr("Modrý");
                loser = tr("červený");
            }
            if (win != empty && !Cube.halted())
            {
                QMessageBox::information(this, tr("Vítěz"), winner + tr(" hráč vyhrál! Blahopřeji! A ") +
                    loser + tr(" nechť nezoufá, příště určitě vyhraje on ;-) ."));
                Cube.halt();
            }
        }
    }
}

void GLFrame::mouseReleaseEvent(QMouseEvent* event)
{
    // delete stored mouse coordinates
    if (!(event->buttons() & Qt::LeftButton))
        X0 = Y0 = -1;
}

void GLFrame::wheelEvent(QWheelEvent* event)
{
    // (un)zoom and redraw
    Cam.R *= powf(0.9, event->delta() / 120.0);
    updateGL();
}
