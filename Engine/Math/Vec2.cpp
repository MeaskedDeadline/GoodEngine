#include "Vec2.h"

namespace Good
{

Vec2::Vec2(float32 s) : x(s), y(s) {}
Vec2::Vec2(float32 x, float32 y) : x(x), y(y) {}

Vec2 Vec2::operator+(const Vec2& v) const { return {x+v.x, y+v.y}; }
Vec2 Vec2::operator-(const Vec2& v) const { return {x-v.x, y-v.y}; }
Vec2 Vec2::operator*(float32 s)     const { return {x*s, y*s}; }
Vec2 Vec2::operator/(float32 s)     const { float32 i=1.0f/s; return {x*i, y*i}; }
Vec2 Vec2::operator*(const Vec2& v) const { return {x*v.x, y*v.y}; }
Vec2 Vec2::operator-()              const { return {-x, -y}; }

Vec2& Vec2::operator+=(const Vec2& v) { x+=v.x; y+=v.y; return *this; }
Vec2& Vec2::operator-=(const Vec2& v) { x-=v.x; y-=v.y; return *this; }
Vec2& Vec2::operator*=(float32 s)     { x*=s; y*=s; return *this; }
Vec2& Vec2::operator/=(float32 s)     { float32 i=1.0f/s; x*=i; y*=i; return *this; }

bool Vec2::operator==(const Vec2& v) const { return Math::Approximately(x,v.x) && Math::Approximately(y,v.y); }
bool Vec2::operator!=(const Vec2& v) const { return !(*this==v); }

float32& Vec2::operator[](int32 i)       { return (&x)[i]; }
float32  Vec2::operator[](int32 i) const { return (&x)[i]; }

float32 Vec2::Length()        const { return Math::Sqrt(x*x+y*y); }
float32 Vec2::LengthSquared() const { return x*x+y*y; }

Vec2 Vec2::Normalized() const
{
    float32 l = Length();
    if (l < Math::EPSILON) return Zero();
    return *this / l;
}

void Vec2::Normalize()
{
    float32 l = Length();
    if (l < Math::EPSILON) { x=0; y=0; return; }
    *this /= l;
}

float32 Vec2::Dot(const Vec2& v)      const { return x*v.x+y*v.y; }
float32 Vec2::Cross(const Vec2& v)    const { return x*v.y-y*v.x; }
float32 Vec2::Distance(const Vec2& v) const { return (*this-v).Length(); }

float32 Vec2::Angle(const Vec2& v) const
{
    float32 d=Dot(v), l=Length()*v.Length();
    if (l < Math::EPSILON) return 0.0f;
    return Math::Acos(Math::Clamp(d/l,-1.0f,1.0f));
}

Vec2 Vec2::Lerp(const Vec2& to, float32 t) const { return {Math::Lerp(x,to.x,t), Math::Lerp(y,to.y,t)}; }
Vec2 Vec2::Reflect(const Vec2& n)           const { return *this - n*(2.0f*Dot(n)); }
Vec2 Vec2::Perpendicular()                  const { return {-y, x}; }

Vec2 Vec2::Zero()  { return {0,0}; }
Vec2 Vec2::One()   { return {1,1}; }
Vec2 Vec2::Up()    { return {0,1}; }
Vec2 Vec2::Down()  { return {0,-1}; }
Vec2 Vec2::Left()  { return {-1,0}; }
Vec2 Vec2::Right() { return {1,0}; }

Vec2 operator*(float32 s, const Vec2& v) { return v*s; }

}