#include "Mat4.h"
#include "Vec3.h"
#include "Vec4.h"
#include "Quat.h"
#include <cstring>

namespace Good
{

Mat4::Mat4() { std::memset(m,0,sizeof(m)); }

Mat4::Mat4(float32 d) { std::memset(m,0,sizeof(m)); m[0][0]=d;m[1][1]=d;m[2][2]=d;m[3][3]=d; }

Mat4::Mat4(const float32* d) { std::memcpy(m,d,16*sizeof(float32)); }

Mat4 Mat4::operator*(const Mat4& o) const
{
    Mat4 r;
    for(int c=0;c<4;c++) for(int row=0;row<4;row++)
        r.m[c][row]=m[0][row]*o.m[c][0]+m[1][row]*o.m[c][1]+m[2][row]*o.m[c][2]+m[3][row]*o.m[c][3];
    return r;
}

Vec4 Mat4::operator*(const Vec4& v) const
{
    return {m[0][0]*v.x+m[1][0]*v.y+m[2][0]*v.z+m[3][0]*v.w,
            m[0][1]*v.x+m[1][1]*v.y+m[2][1]*v.z+m[3][1]*v.w,
            m[0][2]*v.x+m[1][2]*v.y+m[2][2]*v.z+m[3][2]*v.w,
            m[0][3]*v.x+m[1][3]*v.y+m[2][3]*v.z+m[3][3]*v.w};
}

Mat4 Mat4::operator*(float32 s) const { Mat4 r; for(int i=0;i<4;i++) for(int j=0;j<4;j++) r.m[i][j]=m[i][j]*s; return r; }
Mat4 Mat4::operator+(const Mat4& o) const { Mat4 r; for(int i=0;i<4;i++) for(int j=0;j<4;j++) r.m[i][j]=m[i][j]+o.m[i][j]; return r; }
Mat4& Mat4::operator*=(const Mat4& o) { *this=*this*o; return *this; }

bool Mat4::operator==(const Mat4& o) const { for(int i=0;i<4;i++) for(int j=0;j<4;j++) if(!Math::Approximately(m[i][j],o.m[i][j])) return false; return true; }
bool Mat4::operator!=(const Mat4& o) const { return !(*this==o); }

float32& Mat4::operator()(int32 c, int32 r) { return m[c][r]; }
float32  Mat4::operator()(int32 c, int32 r) const { return m[c][r]; }

Mat4 Mat4::Transposed() const { Mat4 r; for(int i=0;i<4;i++) for(int j=0;j<4;j++) r.m[i][j]=m[j][i]; return r; }

float32 Mat4::Determinant() const
{
    float32 a00=m[0][0],a01=m[0][1],a02=m[0][2],a03=m[0][3];
    float32 a10=m[1][0],a11=m[1][1],a12=m[1][2],a13=m[1][3];
    float32 a20=m[2][0],a21=m[2][1],a22=m[2][2],a23=m[2][3];
    float32 a30=m[3][0],a31=m[3][1],a32=m[3][2],a33=m[3][3];
    return a00*(a11*(a22*a33-a23*a32)-a12*(a21*a33-a23*a31)+a13*(a21*a32-a22*a31))
          -a01*(a10*(a22*a33-a23*a32)-a12*(a20*a33-a23*a30)+a13*(a20*a32-a22*a30))
          +a02*(a10*(a21*a33-a23*a31)-a11*(a20*a33-a23*a30)+a13*(a20*a31-a21*a30))
          -a03*(a10*(a21*a32-a22*a31)-a11*(a20*a32-a22*a30)+a12*(a20*a31-a21*a30));
}

Mat4 Mat4::Inverted() const
{
    float32 det=Determinant();
    if(Math::Abs(det)<Math::EPSILON) return Identity();
    float32 id=1.0f/det;
    float32 a[16]; std::memcpy(a,m,sizeof(a));
    Mat4 inv;
    inv.m[0][0]= (a[5]*(a[10]*a[15]-a[11]*a[14])-a[9]*(a[6]*a[15]-a[7]*a[14])+a[13]*(a[6]*a[11]-a[7]*a[10]))*id;
    inv.m[1][0]=-(a[4]*(a[10]*a[15]-a[11]*a[14])-a[8]*(a[6]*a[15]-a[7]*a[14])+a[12]*(a[6]*a[11]-a[7]*a[10]))*id;
    inv.m[2][0]= (a[4]*(a[9]*a[15]-a[11]*a[13])-a[8]*(a[5]*a[15]-a[7]*a[13])+a[12]*(a[5]*a[11]-a[7]*a[9]))*id;
    inv.m[3][0]=-(a[4]*(a[9]*a[14]-a[10]*a[13])-a[8]*(a[5]*a[14]-a[6]*a[13])+a[12]*(a[5]*a[10]-a[6]*a[9]))*id;
    inv.m[0][1]=-(a[1]*(a[10]*a[15]-a[11]*a[14])-a[9]*(a[2]*a[15]-a[3]*a[14])+a[13]*(a[2]*a[11]-a[3]*a[10]))*id;
    inv.m[1][1]= (a[0]*(a[10]*a[15]-a[11]*a[14])-a[8]*(a[2]*a[15]-a[3]*a[14])+a[12]*(a[2]*a[11]-a[3]*a[10]))*id;
    inv.m[2][1]=-(a[0]*(a[9]*a[15]-a[11]*a[13])-a[8]*(a[1]*a[15]-a[3]*a[13])+a[12]*(a[1]*a[11]-a[3]*a[9]))*id;
    inv.m[3][1]= (a[0]*(a[9]*a[14]-a[10]*a[13])-a[8]*(a[1]*a[14]-a[2]*a[13])+a[12]*(a[1]*a[10]-a[2]*a[9]))*id;
    inv.m[0][2]= (a[1]*(a[6]*a[15]-a[7]*a[14])-a[5]*(a[2]*a[15]-a[3]*a[14])+a[13]*(a[2]*a[7]-a[3]*a[6]))*id;
    inv.m[1][2]=-(a[0]*(a[6]*a[15]-a[7]*a[14])-a[4]*(a[2]*a[15]-a[3]*a[14])+a[12]*(a[2]*a[7]-a[3]*a[6]))*id;
    inv.m[2][2]= (a[0]*(a[5]*a[15]-a[7]*a[13])-a[4]*(a[1]*a[15]-a[3]*a[13])+a[12]*(a[1]*a[7]-a[3]*a[5]))*id;
    inv.m[3][2]=-(a[0]*(a[5]*a[14]-a[6]*a[13])-a[4]*(a[1]*a[14]-a[2]*a[13])+a[12]*(a[1]*a[6]-a[2]*a[5]))*id;
    inv.m[0][3]=-(a[1]*(a[6]*a[11]-a[7]*a[10])-a[5]*(a[2]*a[11]-a[3]*a[10])+a[9]*(a[2]*a[7]-a[3]*a[6]))*id;
    inv.m[1][3]= (a[0]*(a[6]*a[11]-a[7]*a[10])-a[4]*(a[2]*a[11]-a[3]*a[10])+a[8]*(a[2]*a[7]-a[3]*a[6]))*id;
    inv.m[2][3]=-(a[0]*(a[5]*a[11]-a[7]*a[9])-a[4]*(a[1]*a[11]-a[3]*a[9])+a[8]*(a[1]*a[7]-a[3]*a[5]))*id;
    inv.m[3][3]= (a[0]*(a[5]*a[10]-a[6]*a[9])-a[4]*(a[1]*a[10]-a[2]*a[9])+a[8]*(a[1]*a[6]-a[2]*a[5]))*id;
    return inv;
}

Vec3 Mat4::GetTranslation() const { return {m[3][0],m[3][1],m[3][2]}; }
Vec3 Mat4::GetScale() const { return {Vec3(m[0][0],m[0][1],m[0][2]).Length(),Vec3(m[1][0],m[1][1],m[1][2]).Length(),Vec3(m[2][0],m[2][1],m[2][2]).Length()}; }
const float32* Mat4::Data() const { return &m[0][0]; }

Mat4 Mat4::Identity() { return Mat4(1.0f); }

Mat4 Mat4::Translate(const Vec3& t) { Mat4 r(1.0f); r.m[3][0]=t.x;r.m[3][1]=t.y;r.m[3][2]=t.z; return r; }
Mat4 Mat4::Scale(const Vec3& s) { Mat4 r(1.0f); r.m[0][0]=s.x;r.m[1][1]=s.y;r.m[2][2]=s.z; return r; }

Mat4 Mat4::RotateX(float32 rad) { Mat4 r(1.0f); float32 c=Math::Cos(rad),s=Math::Sin(rad); r.m[1][1]=c;r.m[2][1]=-s;r.m[1][2]=s;r.m[2][2]=c; return r; }
Mat4 Mat4::RotateY(float32 rad) { Mat4 r(1.0f); float32 c=Math::Cos(rad),s=Math::Sin(rad); r.m[0][0]=c;r.m[2][0]=s;r.m[0][2]=-s;r.m[2][2]=c; return r; }
Mat4 Mat4::RotateZ(float32 rad) { Mat4 r(1.0f); float32 c=Math::Cos(rad),s=Math::Sin(rad); r.m[0][0]=c;r.m[1][0]=-s;r.m[0][1]=s;r.m[1][1]=c; return r; }

Mat4 Mat4::Rotate(const Vec3& axis, float32 rad)
{
    Vec3 a=axis.Normalized();
    float32 c=Math::Cos(rad),s=Math::Sin(rad),t=1-c;
    Mat4 r(1.0f);
    r.m[0][0]=t*a.x*a.x+c;     r.m[0][1]=t*a.x*a.y+s*a.z; r.m[0][2]=t*a.x*a.z-s*a.y;
    r.m[1][0]=t*a.x*a.y-s*a.z; r.m[1][1]=t*a.y*a.y+c;     r.m[1][2]=t*a.y*a.z+s*a.x;
    r.m[2][0]=t*a.x*a.z+s*a.y; r.m[2][1]=t*a.y*a.z-s*a.x; r.m[2][2]=t*a.z*a.z+c;
    return r;
}

Mat4 Mat4::Perspective(float32 fovY,float32 aspect,float32 n,float32 f)
{
    float32 th=Math::Tan(fovY*0.5f); Mat4 r;
    r.m[0][0]=1.0f/(aspect*th); r.m[1][1]=1.0f/th;
    r.m[2][2]=-(f+n)/(f-n); r.m[2][3]=-1.0f; r.m[3][2]=-(2*f*n)/(f-n);
    return r;
}

Mat4 Mat4::Ortho(float32 l,float32 r,float32 b,float32 t,float32 n,float32 f)
{
    Mat4 res(1.0f);
    res.m[0][0]=2.0f/(r-l); res.m[1][1]=2.0f/(t-b); res.m[2][2]=-2.0f/(f-n);
    res.m[3][0]=-(r+l)/(r-l); res.m[3][1]=-(t+b)/(t-b); res.m[3][2]=-(f+n)/(f-n);
    return res;
}

Mat4 Mat4::LookAt(const Vec3& eye,const Vec3& target,const Vec3& worldUp)
{
    Vec3 f=(target-eye).Normalized(), r=f.Cross(worldUp).Normalized(), u=r.Cross(f);
    Mat4 res(1.0f);
    res.m[0][0]=r.x;  res.m[1][0]=r.y;  res.m[2][0]=r.z;
    res.m[0][1]=u.x;  res.m[1][1]=u.y;  res.m[2][1]=u.z;
    res.m[0][2]=-f.x; res.m[1][2]=-f.y; res.m[2][2]=-f.z;
    res.m[3][0]=-r.Dot(eye); res.m[3][1]=-u.Dot(eye); res.m[3][2]=f.Dot(eye);
    return res;
}

Mat4 Mat4::FromQuat(const Quat& q)
{
    float32 xx=q.x*q.x,yy=q.y*q.y,zz=q.z*q.z;
    float32 xy=q.x*q.y,xz=q.x*q.z,yz=q.y*q.z;
    float32 wx=q.w*q.x,wy=q.w*q.y,wz=q.w*q.z;
    Mat4 r(1.0f);
    r.m[0][0]=1-2*(yy+zz); r.m[0][1]=2*(xy+wz);  r.m[0][2]=2*(xz-wy);
    r.m[1][0]=2*(xy-wz);   r.m[1][1]=1-2*(xx+zz); r.m[1][2]=2*(yz+wx);
    r.m[2][0]=2*(xz+wy);   r.m[2][1]=2*(yz-wx);   r.m[2][2]=1-2*(xx+yy);
    return r;
}

Mat4 Mat4::TRS(const Vec3& t,const Quat& rot,const Vec3& s)
{
    return Translate(t)*FromQuat(rot)*Scale(s);
}

}