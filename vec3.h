///////////////////////////////////////////////
// "Piškvorky"
// Jakub Benda (c) 2009

#ifndef _VEC3_H_
#define _VEC3_H_

///
/// \class vec3 - 3D vector (template)
///
template <typename T> class vec3
{
    public:
        vec3() : x(0), y(0), z(0) {}
        vec3(T a, T b, T c) : x(a), y(b), z(c) {}
        vec3(T* array) : x(array[0]), y(array[1]), z(array[2]) {}
        vec3(const vec3& v) : x(v.x), y(v.y), z(v.z) {}

        vec3<T> operator = (const vec3<T>& u)
        {
            x = u.x; y = u.y; z = u.z;
            return *this;
        }

        vec3<T> operator += (const vec3<T> u) { return *this = *this + u; }
        vec3<T> operator -= (const vec3<T> u) { return *this = *this - u; }

        T x, y, z;
};

///////////////////////////////////////////////
// arithmetic operators
template <typename T> T operator | (vec3<T> u, vec3<T> v) { return u.x * v.x + u.y * v.y + u.z * v.z; }
template <typename T> vec3<T> operator + (vec3<T> u, vec3<T> v) { return vec3<T>(u.x+v.x, u.y+v.y, u.z+v.z); }
template <typename T> vec3<T> operator - (vec3<T> u, vec3<T> v) { return vec3<T>(u.x-v.x, u.y-v.y, u.z-v.z); }
template <typename T> vec3<T> operator * (vec3<T> u, T a) { return vec3<T>(u.x * a, u.y * a, u.z * a); }
template <typename T> vec3<T> operator * (T a, vec3<T> u) { return vec3<T>(u.x * a, u.y * a, u.z * a); }
template <typename T> vec3<T> operator / (vec3<T> u, T a) { return vec3<T>(u.x / a, u.y / a, u.z / a); }
template <typename T> vec3<T> operator / (vec3<T> u, vec3<T> v) { return vec3<T>(u.x/v.x, u.y/v.y, u.z/v.z); }

///////////////////////////////////////////
// comparators
template <typename T> vec3<bool> operator == (vec3<T> u, vec3<T> v) { return vec3<bool>(u.x == v.x, u.y == v.y, u.z == v.z); }
template <typename T> vec3<bool> operator != (vec3<T> u, vec3<T> v) { return vec3<bool>(u.x != v.x, u.y != v.y, u.z != v.z); }
template <typename T> vec3<bool> operator > (vec3<T> u, vec3<T> v) { return vec3<bool>(u.x > v.x, u.y > v.y, u.z > v.z); }
template <typename T> vec3<bool> operator < (vec3<T> u, vec3<T> v) { return vec3<bool>(u.x < v.x, u.y < v.y, u.z < v.z); }
template <typename T> vec3<bool> operator >= (vec3<T> u, vec3<T> v) { return vec3<bool>(u.x >= v.x, u.y >= v.y, u.z >= v.z); }
template <typename T> vec3<bool> operator <= (vec3<T> u, vec3<T> v) { return vec3<bool>(u.x <= v.x, u.y <= v.y, u.z <= v.z); }
template <typename T> vec3<bool> operator == (vec3<T> u, T a) { return u == vec3<T>(a,a,a); }
template <typename T> vec3<bool> operator != (vec3<T> u, T a) { return u != vec3<T>(a,a,a); }
template <typename T> vec3<bool> operator > (vec3<T> u, T a) { return u > vec3<T>(a,a,a); }
template <typename T> vec3<bool> operator < (vec3<T> u, T a) { return u < vec3<T>(a,a,a); }
template <typename T> vec3<bool> operator >= (vec3<T> u, T a) { return u >= vec3<T>(a,a,a); }
template <typename T> vec3<bool> operator <= (vec3<T> u, T a) { return u <= vec3<T>(a,a,a); }

/////////////////////////////////////////////
// agregators
inline bool every(vec3<bool> b) { return b.x && b.y && b.z; }
inline bool any(vec3<bool> b) { return b.x || b.y || b.z; }

////////////////////////////////////////////////
// other
template <typename T> vec3<T> max(vec3<T> u, vec3<T> v)
{
    return vec3<T>(u.x > v.x ? u.x : v.x, u.y > v.y ? u.y : v.y, u.z > v.z ? u.z : v.z);
}

template <typename T> vec3<T> min(vec3<T> u, vec3<T> v)
{
    return vec3<T>(u.x < v.x ? u.x : v.x, u.y < v.y ? u.y : v.y, u.z < v.z ? u.z : v.z);
}

#endif
