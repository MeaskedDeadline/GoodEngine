#include "Color.h"

namespace Good
{

Color::Color(float32 r,float32 g,float32 b,float32 a):r(r),g(g),b(b),a(a){}

Color Color::operator*(float32 s)      const { return {r*s,g*s,b*s,a*s}; }
Color Color::operator+(const Color& c) const { return {r+c.r,g+c.g,b+c.b,a+c.a}; }
Color Color::operator*(const Color& c) const { return {r*c.r,g*c.g,b*c.b,a*c.a}; }

bool Color::operator==(const Color& c) const { return Math::Approximately(r,c.r)&&Math::Approximately(g,c.g)&&Math::Approximately(b,c.b)&&Math::Approximately(a,c.a); }
bool Color::operator!=(const Color& c) const { return !(*this==c); }

uint32 Color::ToRGBA8() const
{
    uint8 ri=static_cast<uint8>(Math::Clamp(r,0.0f,1.0f)*255);
    uint8 gi=static_cast<uint8>(Math::Clamp(g,0.0f,1.0f)*255);
    uint8 bi=static_cast<uint8>(Math::Clamp(b,0.0f,1.0f)*255);
    uint8 ai=static_cast<uint8>(Math::Clamp(a,0.0f,1.0f)*255);
    return (ri<<24)|(gi<<16)|(bi<<8)|ai;
}

uint32 Color::ToABGR8() const
{
    uint8 ri=static_cast<uint8>(Math::Clamp(r,0.0f,1.0f)*255);
    uint8 gi=static_cast<uint8>(Math::Clamp(g,0.0f,1.0f)*255);
    uint8 bi=static_cast<uint8>(Math::Clamp(b,0.0f,1.0f)*255);
    uint8 ai=static_cast<uint8>(Math::Clamp(a,0.0f,1.0f)*255);
    return (ai<<24)|(bi<<16)|(gi<<8)|ri;
}

Color Color::Lerp(const Color& to, float32 t) const
{
    return {Math::Lerp(r,to.r,t),Math::Lerp(g,to.g,t),Math::Lerp(b,to.b,t),Math::Lerp(a,to.a,t)};
}

Color Color::FromRGBA8(uint32 rgba)
{
    return {static_cast<float32>((rgba>>24)&0xFF)/255.0f,
            static_cast<float32>((rgba>>16)&0xFF)/255.0f,
            static_cast<float32>((rgba>>8)&0xFF)/255.0f,
            static_cast<float32>(rgba&0xFF)/255.0f};
}

Color Color::FromHSV(float32 h,float32 s,float32 v,float32 a)
{
    h=Math::Fmod(h,360.0f); if(h<0) h+=360;
    float32 c=v*s, x=c*(1-Math::Abs(Math::Fmod(h/60,2)-1)), m=v-c;
    float32 ri,gi,bi;
    if(h<60){ri=c;gi=x;bi=0;}
    else if(h<120){ri=x;gi=c;bi=0;}
    else if(h<180){ri=0;gi=c;bi=x;}
    else if(h<240){ri=0;gi=x;bi=c;}
    else if(h<300){ri=x;gi=0;bi=c;}
    else{ri=c;gi=0;bi=x;}
    return {ri+m,gi+m,bi+m,a};
}

Color Color::White()   { return {1,1,1,1}; }
Color Color::Black()   { return {0,0,0,1}; }
Color Color::Red()     { return {1,0,0,1}; }
Color Color::Green()   { return {0,1,0,1}; }
Color Color::Blue()    { return {0,0,1,1}; }
Color Color::Yellow()  { return {1,1,0,1}; }
Color Color::Cyan()    { return {0,1,1,1}; }
Color Color::Magenta() { return {1,0,1,1}; }
Color Color::Gray()    { return {0.5f,0.5f,0.5f,1}; }
Color Color::Clear()   { return {0,0,0,0}; }

}