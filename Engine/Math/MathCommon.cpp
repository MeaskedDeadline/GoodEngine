#include "MathCommon.h"

namespace Good
{
namespace Math
{
    float32 ToRadians(float32 d) { return d * DEG2RAD; }
    float32 ToDegrees(float32 r) { return r * RAD2DEG; }
    float32 Abs(float32 x)      { return std::fabs(x);  }
    float32 Floor(float32 x)    { return std::floor(x);  }
    float32 Ceil(float32 x)     { return std::ceil(x);   }
    float32 Round(float32 x)    { return std::round(x);  }
    float32 Sqrt(float32 x)     { return std::sqrt(x);   }
    float32 InvSqrt(float32 x)  { return 1.0f / std::sqrt(x); }
    float32 Sin(float32 x)      { return std::sin(x);   }
    float32 Cos(float32 x)      { return std::cos(x);   }
    float32 Tan(float32 x)      { return std::tan(x);   }
    float32 Asin(float32 x)     { return std::asin(x);  }
    float32 Acos(float32 x)     { return std::acos(x);  }
    float32 Atan2(float32 y, float32 x) { return std::atan2(y, x); }
    float32 Pow(float32 b, float32 e)   { return std::pow(b, e);   }
    float32 Log(float32 x)      { return std::log(x);   }
    float32 Exp(float32 x)      { return std::exp(x);   }
    float32 Fmod(float32 x, float32 y)  { return std::fmod(x, y); }
    float32 Min(float32 a, float32 b)   { return (a < b) ? a : b; }
    float32 Max(float32 a, float32 b)   { return (a > b) ? a : b; }

    float32 Clamp(float32 x, float32 lo, float32 hi)
    {
        if (x < lo) return lo;
        if (x > hi) return hi;
        return x;
    }

    float32 Lerp(float32 a, float32 b, float32 t)
    {
        return a + (b - a) * t;
    }

    float32 Smoothstep(float32 e0, float32 e1, float32 x)
    {
        float32 t = Clamp((x - e0) / (e1 - e0), 0.0f, 1.0f);
        return t * t * (3.0f - 2.0f * t);
    }

    float32 InverseLerp(float32 a, float32 b, float32 v)
    {
        if (Approximately(a, b)) return 0.0f;
        return (v - a) / (b - a);
    }

    bool Approximately(float32 a, float32 b, float32 eps)
    {
        return Abs(a - b) <= eps;
    }

    int32 Sign(float32 x)
    {
        if (x > 0.0f) return 1;
        if (x < 0.0f) return -1;
        return 0;
    }

    float32 Snap(float32 v, float32 grid)
    {
        if (grid <= 0.0f) return v;
        return Round(v / grid) * grid;
    }
}
}