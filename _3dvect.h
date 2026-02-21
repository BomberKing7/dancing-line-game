#include <math.h>
#define num float
struct vect
{
    num a = 0, b = 0, c = 0;

    num dot(vect x)
    {
        return a * x.a + b * x.b + c * x.c;
    }
    vect dot(num x)
    {
        return vect{a * x, b * x, c * x};
    }
    vect cross(vect x)
    {
        return vect{x.c * b - x.b * c, -(x.c * a - x.a * c), (x.b * a - x.a * b)};
    }
    vect add(vect x)
    {
        return vect{a + x.a, b + x.b, c + x.c};
    }
    vect sub(vect x)
    {
        return vect{a - x.a, b - x.b, c - x.c};
    }
    num length()
    {
        return sqrt(a * a + b * b + c * c);
    }
    vect normal()
    {
        return dot(1 / length());
    }
    vect Rotate(vect x, num t)
    {
        x = x.normal();
        vect get = vect{a, b, c};
        get = (x.dot(get.dot(x)).dot(1 - cos(t))).add(get.dot(cos(t)).add(x.cross(get).dot(sin(t))));
        return get;
    }
    vect onPlane()
    {
        return vect{a, b, 0};
    }
};
