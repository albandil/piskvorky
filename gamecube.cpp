///////////////////////////////////////////////
// "Piškvorky"
// Jakub Benda (c) 2009

#include <QtOpenGL>

#include "camera.h"
#include "gamecube.h"
#include "utils.h"

bool gamecube::distance_fn(const planeinfo& plane1, const planeinfo& plane2)
{
    return Cam.distanceFrom(plane1.Parent->Pos + EDGE/2 * plane1.Nrm)
        > Cam.distanceFrom(plane2.Parent->Pos + EDGE/2 * plane2.Nrm);
}

gamecube::gamecube()
{
    Dimension = 0;
    Disperse = DrawForPicking = Freeze = false;
    WhoseTurn = circle;
}

gamecube::gamecube(unsigned dim)
{
    Dimension = dim;
    Disperse = DrawForPicking = Freeze = false;
    WhoseTurn = circle;

    // reserve space for arrays
    unsigned N = dim * dim * dim;
    Elems.reserve(N);
    Planes.reserve(N);

    // compute coordinates of all planes
    setupPlanes();
}

void gamecube::setupPlanes()
{
    // setup planes
    for (unsigned x = 0; x < Dimension; x++)
    {
        for (unsigned y = 0; y < Dimension; y++)
        {
            for (unsigned z = 0; z < Dimension; z++)
            {
                unsigned i = (x * Dimension + y) * Dimension + z;

                smallcube Elem;
                Elem.Pos = vec3<float>(x, y, z) - (Dimension - 1) * EDGE/2 * vec3<float>(1,1,1);
                Elem.Type = empty;
                Elem.Picked = false;
                Elem.Colour = 2 * i + 1;
                Elems.push_back(Elem);

                setupPlane(i, 0,  1,  0,  0);
                setupPlane(i, 1,  0,  1,  0);
                setupPlane(i, 2,  0,  0,  1);
                setupPlane(i, 3, -1,  0,  0);
                setupPlane(i, 4,  0, -1,  0);
                setupPlane(i, 5,  0,  0, -1);
            }
        }
    }
}

void gamecube::reserveColour(const QColor& R)
{
    // prevents usage of background color with red component conflicting with some picking colour
    for (std::vector<smallcube>::iterator Elem = Elems.begin(); Elem != Elems.end(); Elem++)
        if (Elem->Colour == (unsigned)R.red())
            Elem->Colour++;
}

void gamecube::switchDisperse()
{
    Disperse = !Disperse;
}

void gamecube::updateVisibility()
{
    // sort according to distance from camera
    std::sort(Planes.begin(), Planes.end(), &distance_fn);
}

void gamecube::draw()
{
    float span = 1;
    if (Disperse)
        span = 2;

    // draws planes
    for (std::vector<planeinfo>::const_iterator it = Planes.begin(); it != Planes.end(); it++)
        drawPlane(span * it->Parent->Pos + it->Nrm * EDGE/2.0f, it->Nrm,
                  it->Parent->Type, it->Parent->Picked, it->Parent->Colour);
}

void gamecube::setupPlane(unsigned i, unsigned s, float nx, float ny, float nz)
{
    Q_UNUSED(s);

    planeinfo Plane;
    Plane.Parent = &(Elems[i]);
    Plane.Nrm.x = nx;
    Plane.Nrm.y = ny;
    Plane.Nrm.z = nz;
    Planes.push_back(Plane);
}

void gamecube::drawPlane(vec3<float> r, vec3<float> n, tsign type, bool picked, unsigned char clr)
{
    glPushMatrix();
    glTranslatef(r.x, r.y, r.z);

    glBegin(GL_QUADS);
        if (DrawForPicking)
            glColor3ub(clr, 0, 0);
        else if (picked)
            glColor4f(type == cross ? 1 : 0.95, 0.95, type == circle ? 1 : 0.95, type == empty ? 0.15 : 0.55);
        else
            glColor4f(type == cross ? 1 : 0.75, 0.75, type == circle ? 1 : 0.75, type == empty ? 0.15 : 0.55);

        float d = 0.45;

        if (n.x > 0)
        {
            glVertex3f(-n.x*(1-2*d)/2, -d, -d);    glVertex3f(-n.x*(1-2*d)/2, d, -d);
            glVertex3f(-n.x*(1-2*d)/2, d, d);    glVertex3f(-n.x*(1-2*d)/2, -d, d);
        }

        if (n.x < 0)
        {
            glVertex3f(-n.x*(1-2*d)/2, -d, -d);    glVertex3f(-n.x*(1-2*d)/2, -d, d);
            glVertex3f(-n.x*(1-2*d)/2, d, d);    glVertex3f(-n.x*(1-2*d)/2, d, -d);
        }

        if (n.y > 0)
        {
            glVertex3f(-d, -n.y*(1-2*d)/2, -d);    glVertex3f(-d, -n.y*(1-2*d)/2, d);
            glVertex3f(d, -n.y*(1-2*d)/2, d);    glVertex3f(d, -n.y*(1-2*d)/2, -d);
        }

        if (n.y < 0)
        {
            glVertex3f(-d, -n.y*(1-2*d)/2, -d);    glVertex3f(d, -n.y*(1-2*d)/2, -d);
            glVertex3f(d, -n.y*(1-2*d)/2, d);    glVertex3f(-d, -n.y*(1-2*d)/2, d);
        }

        if (n.z > 0)
        {
            glVertex3f(-d, -d, -n.z*(1-2*d)/2);    glVertex3f(d, -d, -n.z*(1-2*d)/2);
            glVertex3f(d, d, -n.z*(1-2*d)/2);    glVertex3f(-d, d, -n.z*(1-2*d)/2);
        }

        if (n.z < 0)
        {
            glVertex3f(-d, -d, -n.z*(1-2*d)/2);    glVertex3f(-d, d, -n.z*(1-2*d)/2);
            glVertex3f(d, d, -n.z*(1-2*d)/2);    glVertex3f(d, -d, -n.z*(1-2*d)/2);
        }
    glEnd();

    glPopMatrix();
}

bool gamecube::cursorPick(int cx, int cy)
{
    glDisable(GL_BLEND);
    DrawForPicking = true;
    draw();
    DrawForPicking = false;
    glEnable(GL_BLEND);

    glFlush();

    GLint viewport[4];
    glGetIntegerv(GL_VIEWPORT, viewport);

    GLubyte px[3];
    glReadPixels(cx, viewport[3] - cy, 1, 1, GL_RGB, GL_UNSIGNED_BYTE, px);

    bool ret = false;

    for (unsigned int i = 0; i < Elems.size(); i++)
    {
        // was it picked?
        bool prevp = Elems[i].Picked;
        Elems[i].Picked = (Elems[i].Colour == px[0] && px[1] == 0 && px[2] == 0);
        if (Elems[i].Picked != prevp)
            ret = true;
    }

    return ret;
}

bool gamecube::selectPicked()
{
    if (Freeze)
        return false;

    // find the picked node
    for (unsigned i = 0; i < Elems.size(); i++)
    {
        if (Elems[i].Picked && Elems[i].Type == empty)
        {
            Elems[i].Type = WhoseTurn;
            WhoseTurn = (WhoseTurn == circle) ? cross : circle;
            UndoBuffer.push_back(i);
            return true;
        }
    }

    return false;
}

void gamecube::undo()
{
    if (!UndoBuffer.empty() && !Freeze)
    {
        Elems[UndoBuffer.back()].Type = empty;
        UndoBuffer.pop_back();
    }
}

void gamecube::reset()
{
    for (unsigned i = 0; i < Elems.size(); i++)
        Elems[i].Type = empty;
    UndoBuffer.clear();
    Freeze = false;
    WhoseTurn = circle;
}

tsign gamecube::checkWinner() const
{
    // how long the line must be
    static const unsigned length = Dimension;

    // thirteen directions in rectangular 3D-grid
    static const vec3<int> dir[] = {
        vec3<int>(1,0,0),  vec3<int>(0,1,0),  vec3<int>(0,0,1),    // axes
        vec3<int>(1,1,0),  vec3<int>(1,0,1),  vec3<int>(0,1,1),        // wall hypotenuses
        vec3<int>(1,-1,0), vec3<int>(1,0,-1), vec3<int>(0,1,-1),    // wall hypotenuses
        vec3<int>(1,1,1),  vec3<int>(1,1,-1),        // body hypotenuses
        vec3<int>(-1,1,1), vec3<int>(-1,1,-1)        // body hypotenuses
    };

    // for all elements generating the gamecube
    for (int x = 0; x < (int)Dimension; x++)
    {
        for (int y = 0; y < (int)Dimension; y++)
        {
            for (int z = 0; z < (int)Dimension; z++)
            {
                // get the type of current element
                tsign symbol = Elems[getIndex(x,y,z)].Type;
                if (symbol == empty)
                    continue;

                // is element (x,y,z) part of a full line? (of length 'length'); check all 13 directions
                for (int i = 0; i < (int)(sizeof(dir)/sizeof(vec3<int>)); i++)
                {
                    unsigned count_in_line = 0;

                    // check direction 'dir[i]'
                    for (vec3<int> pos(x,y,z); every(pos >= 0) && every(pos < (int)Dimension); pos -= dir[i])
                        if (Elems[getIndex(pos.x,pos.y,pos.z)].Type == symbol)
                            count_in_line++;
                        else
                            break;

                    // check direction '-dir[i]'
                    for (vec3<int> pos(x,y,z); every(pos >= 0) && every(pos < (int)Dimension); pos += dir[i])
                        if (Elems[getIndex(pos.x,pos.y,pos.z)].Type == symbol)
                            count_in_line++;
                        else
                            break;

                    // long enough? (start is reckoned twice, so we need longer line than 'length')
                    if (count_in_line > length)
                        return symbol;
                }
            }
        }
    }
    return empty;
}

unsigned gamecube::getIndex(int x, int y, int z) const
{
    return Dimension * (Dimension * x + y) + z;
}

void gamecube::halt()
{
    Freeze = true;
}

bool gamecube::halted() const
{
    return Freeze;
}
