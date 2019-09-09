///////////////////////////////////////////////
// "Piškvorky"
// Jakub Benda (c) 2009

#include <cmath>

#include "camera.h"
#include "utils.h"

float camera::distanceFrom(const vec3<float>& r) const
{
    return sqrt(_sqr(r.x - pos().x) + _sqr(r.y - pos().y) + _sqr(r.z - pos().z));
}

vec3<float> camera::pos() const
{
     return vec3<float>(x,y,z) + R * vec3<float>(sin(theta)*sin(phi), cos(theta), sin(theta)*cos(phi));
}