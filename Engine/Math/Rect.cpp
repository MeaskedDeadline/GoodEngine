#include "Rect.h"

namespace Good
{

    Rect::Rect(float32 x,float32 y,float32 w,float32 h):x(x),y(y),width(w),height(h){}

    float32 Rect::Left()   const { return x; }
    float32 Rect::Right()  const { return x+width; }
    float32 Rect::Top()    const { return y; }
    float32 Rect::Bottom() const { return y+height; }

    Vec2    Rect::Center() const { return {x+width*0.5f,y+height*0.5f}; }
    Vec2    Rect::Size()   const { return {width,height}; }
    Vec2    Rect::Min()    const { return {x,y}; }
    Vec2    Rect::Max()    const { return {x+width,y+height}; }
    float32 Rect::Area()   const { return width*height; }

    bool Rect::Contains(const Vec2& p) const { return p.x>=x&&p.x<=x+width&&p.y>=y&&p.y<=y+height; }

    bool Rect::Overlaps(const Rect& o) const
    {
        return x<o.x+o.width&&x+width>o.x&&y<o.y+o.height&&y+height>o.y;
    }

    Rect Rect::Expanded(float32 a) const { return {x-a,y-a,width+a*2,height+a*2}; }

    Rect Rect::Merged(const Rect& o) const
    {
        float32 mnx=Math::Min(x,o.x),mny=Math::Min(y,o.y);
        float32 mxx=Math::Max(x+width,o.x+o.width),mxy=Math::Max(y+height,o.y+o.height);
        return {mnx,mny,mxx-mnx,mxy-mny};
    }

    Rect Rect::FromMinMax(const Vec2& mn,const Vec2& mx) { return {mn.x,mn.y,mx.x-mn.x,mx.y-mn.y}; }
    Rect Rect::Zero() { return {0,0,0,0}; }

}