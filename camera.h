///////////////////////////////////////////////
// "Piškvorky"
// Jakub Benda (c) 2009

#ifndef _CAMERA_H_
#define _CAMERA_H_

#include "utils.h"

///
/// \class camera - access to the camera information
///
class camera
{
    public:
        // computes distance from camera to specified point
        float distanceFrom(const vec3<float>&) const;
        vec3<float> pos() const;

        float x, y, z; // target
        float R, theta, phi; // relative position vector
};

extern camera Cam;

#endif
