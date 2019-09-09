///////////////////////////////////////////////
// "Piškvorky"
// Jakub Benda (c) 2009

#ifndef _GAMECUBE_H_
#define _GAMECUBE_H_

#include <algorithm>

#include "camera.h"
#include "utils.h"

#define EDGE 1.0f

// possible value of a node
enum tsign {empty, cross, circle};

///
/// \struct smallcube - contains information about six planes forming a small cube
///
struct smallcube
{
    // position (of center)
    vec3<float> Pos;
    // contains
    tsign Type;
    // whether picked
    bool Picked;
    // identification colour used for picking
    unsigned int Colour;
};

///
/// \struct planeinfo - contains information about a specific plane
///
struct planeinfo
{
    // position (of center) relative to that of parent
    vec3<float> Sft;
    // normal vecor
    vec3<float> Nrm;
    // parent smallcube
    smallcube* Parent;
};

///
/// \class gamecube - holds information about the game field
///
class gamecube
{        
    public:

        // constructors
        gamecube();
        gamecube(unsigned dim);

        // destructor
        ~gamecube() { }

        // creates all planes, of which the cubes are constituted
        void setupPlanes();

        // predicate for distance comparison
        static bool distance_fn(const planeinfo&, const planeinfo&);

        // sorts planes according to distance from camera
        void updateVisibility();

        // prevent usage of specific colour
        void reserveColour(const QColor&);

        // switch disperse status
        void switchDisperse();

        // draws the boxes
        void draw();

        // compute index for use in Elems[] from node coordinates
        unsigned getIndex(int, int, int) const;

        // select node by the cursor position
        bool cursorPick(int, int);

        // select the picked node for current player
        bool selectPicked();

        // undo
        void undo();

        // empty the game field
        void reset();

        // did anyone win already?
        tsign checkWinner() const;

        // disable selecting (e.g. after end of game)
        void halt();
        bool halted() const;

    private:

        // stores information about i-th plane (i.e. the position and the normal)
        void setupPlane(unsigned i, unsigned s, float nx, float ny, float nz);

        // draws one plane
        void drawPlane(vec3<float> pos, vec3<float> nrm, tsign type, bool picked, unsigned char clr);

        // the extent of the cube in number of nodes
        unsigned Dimension;

        // whether or not to draw dispersed
        bool Disperse;

        // whether the cube is frozen
        bool Freeze;

        // whether we draw for the use in picking algorithm only
        // (i.e. no blending, no highlighting)
        bool DrawForPicking;

        // who is now playing?
        tsign WhoseTurn;

        // undo buffer -- contains the selection sequence
        std::vector<unsigned> UndoBuffer;

        // lists containing information about nodes and about planes
        std::vector<smallcube> Elems;
        std::vector<planeinfo> Planes;
};

#endif
