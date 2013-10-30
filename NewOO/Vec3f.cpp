/* Vector3f class */

#ifndef _VEC3F_CPP
#define _VEC3F_CPP 1

class Vec3f
{
    public:
        float x, y, z;
        Vec3f(float x, float y, float z): x(x), y(y), z(z) {}
        Vec3f() { x=0; y=0; z=0; }

    private:
};

#endif
