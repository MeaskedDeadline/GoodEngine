#include "Vec3.h"
#include "Vec2.h"

namespace Good
{

Vec3::Vec3(float32 s) : x(s), y(s), z(s) {}
Vec3::Vec3(float32 x, float32 y, float32 z) : x(x), y(y), z(z) {}
Vec3::Vec3(const Vec2& v, float32 z) : x(v.x), y(v.y), z(z) {}

Vec3 Vec3::operator+(const Vec3& v) const { return {x+v.x,y+v.y,z+v.z}; }
Vec3 Vec3::operator-(const Vec3& v) const { return {x-v.x,y-v.y,z-v.z}; }
Vec3 Vec3::operator*(float32 s)     const { return {x*s,y*s,z*s}; }
Vec3 Vec3::operator/(float32 s)     const { float32 i=1.0f/s; return {x*i,y*i,z*i}; }
Vec3 Vec3::operator*(const Vec3& v) const { return {x*v.x,y*v.y,z*v.z}; }
Vec3 Vec3::operator-()              const { return {-x,-y,-z}; }

Vec3& Vec3::operator+=(const Vec3& v) { x+=v.x; y+=v.y; z+=v.z; return *this; }
Vec3& Vec3::operator-=(const Vec3& v) { x-=v.x; y-=v.y; z-=v.z; return *this; }
Vec3& Vec3::operator*=(float32 s)     { x*=s; y*=s; z*=s; return *this; }
Vec3& Vec3::operator/=(float32 s)     { float32 i=1.0f/s; x*=i; y*=i; z*=i; return *this; }

bool Vec3::operator==(const Vec3& v) const { return Math::Approximately(x,v.x)&&Math::Approximately(y,v.y)&&Math::Approximately(z,v.z); }
bool Vec3::operator!=(const Vec3& v) const { return !(*this==v); }

float32& Vec3::operator[](int32 i)       { return (&x)[i]; }
float32  Vec3::operator[](int32 i) const { return (&x)[i]; }

float32 Vec3::Length()        const { return Math::Sqrt(x*x+y*y+z*z); }
float32 Vec3::LengthSquared() const { return x*x+y*y+z*z; }

Vec3 Vec3::Normalized() const { float32 l=Length(); if(l<Math::EPSILON) return Zero(); return *this/l; }
void Vec3::Normalize()        { float32 l=Length(); if(l<Math::EPSILON){x=y=z=0;return;} *this/=l; }

float32 Vec3::Dot(const Vec3& v)      const { return x*v.x+y*v.y+z*v.z; }
Vec3    Vec3::Cross(const Vec3& v)    const { return {y*v.z-z*v.y, z*v.x-x*v.z, x*v.y-y*v.x}; }
float32 Vec3::Distance(const Vec3& v) const { return (*this-v).Length(); }

float32 Vec3::Angle(const Vec3& v) const
{
    float32 d=Dot(v), l=Length()*v.Length();
    if(l<Math::EPSILON) return 0;
    return Math::Acos(Math::Clamp(d/l,-1.0f,1.0f));
}

Vec3 Vec3::Lerp(const Vec3& to, float32 t) const { return {Math::Lerp(x,to.x,t),Math::Lerp(y,to.y,t),Math::Lerp(z,to.z,t)}; }

Vec3 Vec3::Reflect(const Vec3& n) const { return *this-n*(2.0f*Dot(n)); }

Vec3 Vec3::Project(const Vec3& onto) const
{
    float32 d=onto.Dot(onto);
    if(d<Math::EPSILON) return Zero();
    return onto*(Dot(onto)/d);
}

Vec3 Vec3::Zero()    { return {0,0,0}; }
Vec3 Vec3::One()     { return {1,1,1}; }
Vec3 Vec3::Up()      { return {0,1,0}; }
Vec3 Vec3::Down()    { return {0,-1,0}; }
Vec3 Vec3::Left()    { return {-1,0,0}; }
Vec3 Vec3::Right()   { return {1,0,0}; }
Vec3 Vec3::Forward() { return {0,0,-1}; }
Vec3 Vec3::Back()    { return {0,0,1}; }

Vec3 operator*(float32 s, const Vec3& v) { return v*s; }

}