#pragma once
#include "Core/Core.h"
#include <cmath>
#include <limits>

namespace Good
{
    namespace Math
    {
        constexpr float32 PI        = 3.14159265358979323846f;
        constexpr float32 TWO_PI    = PI * 2.0f;
        constexpr float32 HALF_PI   = PI * 0.5f;
        constexpr float32 DEG2RAD   = PI / 180.0f;
        constexpr float32 RAD2DEG   = 180.0f / PI;
        constexpr float32 EPSILON   = 1e-6f;
        constexpr float32 INFINITY_ = std::numeric_limits<float32>::infinity();

        float32 ToRadians(float32 degrees);
        float32 ToDegrees(float32 radians);
        float32 Abs(float32 x);
        float32 Floor(float32 x);
        float32 Ceil(float32 x);
        float32 Round(float32 x);
        float32 Sqrt(float32 x);
        float32 InvSqrt(float32 x);
        float32 Sin(float32 x);
        float32 Cos(float32 x);
        float32 Tan(float32 x);
        float32 Asin(float32 x);
        float32 Acos(float32 x);
        float32 Atan2(float32 y, float32 x);
        float32 Pow(float32 base, float32 exp);
        float32 Log(float32 x);
        float32 Exp(float32 x);
        float32 Fmod(float32 x, float32 y);
        float32 Min(float32 a, float32 b);
        float32 Max(float32 a, float32 b);
        float32 Clamp(float32 x, float32 lo, float32 hi);
        float32 Lerp(float32 a, float32 b, float32 t);
        float32 Smoothstep(float32 edge0, float32 edge1, float32 x);
        float32 InverseLerp(float32 a, float32 b, float32 value);
        bool    Approximately(float32 a, float32 b, float32 eps = EPSILON);
        int32   Sign(float32 x);
        float32 Snap(float32 value, float32 grid);
    }
}