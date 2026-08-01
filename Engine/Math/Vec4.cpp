#include "Vec4.h"
#include "Vec3.h"

namespace Good
{

    Vec4::Vec4(float32 s) : x(s),y(s),z(s),w(s) {}
    Vec4::Vec4(float32 x,float32 y,float32 z,float32 w) : x(x),y(y),z(z),w(w) {}
    Vec4::Vec4(const Vec3& v,float32 w) : x(v.x),y(v.y),z(v.z),w(w) {}

    Vec4 Vec4::operator+(const Vec4& v) const { return {x+v.x,y+v.y,z+v.z,w+v.w}; }
    Vec4 Vec4::operator-(const Vec4& v) const { return {x-v.x,y-v.y,z-v.z,w-v.w}; }
    Vec4 Vec4::operator*(float32 s)     const { return {x*s,y*s,z*s,w*s}; }
    Vec4 Vec4::operator/(float32 s)     const { float32 i=1.0f/s; return {x*i,y*i,z*i,w*i}; }
    Vec4 Vec4::operator-()              const { return {-x,-y,-z,-w}; }

    Vec4& Vec4::operator+=(const Vec4& v) { x+=v.x;y+=v.y;z+=v.z;w+=v.w; return *this; }
    Vec4& Vec4::operator-=(const Vec4& v) { x-=v.x;y-=v.y;z-=v.z;w-=v.w; return *this; }
    Vec4& Vec4::operator*=(float32 s)     { x*=s;y*=s;z*=s;w*=s; return *this; }
    Vec4& Vec4::operator/=(float32 s)     { float32 i=1.0f/s; x*=i;y*=i;z*=i;w*=i; return *this; }

    bool Vec4::operator==(const Vec4& v) const { return Math::Approximately(x,v.x)&&Math::Approximately(y,v.y)&&Math::Approximately(z,v.z)&&Math::Approximately(w,v.w); }
    bool Vec4::operator!=(const Vec4& v) const { return !(*this==v); }

    float32& Vec4::operator[](int32 i)       { return (&x)[i]; }
    float32  Vec4::operator[](int32 i) const { return (&x)[i]; }

    float32 Vec4::Length()        const { return Math::Sqrt(x*x+y*y+z*z+w*w); }
    float32 Vec4::LengthSquared() const { return x*x+y*y+z*z+w*w; }

    Vec4 Vec4::Normalized() const { float32 l=Length(); if(l<Math::EPSILON) return Zero(); return *this/l; }
    void Vec4::Normalize()        { float32 l=Length(); if(l<Math::EPSILON){x=y=z=w=0;return;} *this/=l; }

    float32 Vec4::Dot(const Vec4& v) const { return x*v.x+y*v.y+z*v.z+w*v.w; }
    Vec3    Vec4::XYZ()              const { return {x,y,z}; }

    Vec4 Vec4::Zero() { return {0,0,0,0}; }
    Vec4 Vec4::One()  { return {1,1,1,1}; }

    Vec4 operator*(float32 s, const Vec4& v) { return v*s; }

}