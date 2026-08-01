#include "Random.h"
#include <cmath>
#include <limits>

namespace Good
{

    uint64 Random::s_State = 12345678901234567ull;

    void Random::Seed(uint64 seed) { s_State = seed ? seed : 1ull; }

    uint64 Random::Next()
    {
        // xoshiro256** inspired simple xorshift64
        uint64 x = s_State;
        x ^= x << 13;
        x ^= x >> 7;
        x ^= x << 17;
        s_State = x;
        return x;
    }

    int32 Random::Int(int32 min, int32 max)
    {
        if (min >= max) return min;
        return min + static_cast<int32>(Next() % static_cast<uint64>(max - min + 1));
    }

    float32 Random::Float(float32 min, float32 max)
    {
        float32 t = static_cast<float32>(Next()) /
                    static_cast<float32>(std::numeric_limits<uint64>::max());
        return min + t * (max - min);
    }

    float64 Random::Double(float64 min, float64 max)
    {
        float64 t = static_cast<float64>(Next()) /
                    static_cast<float64>(std::numeric_limits<uint64>::max());
        return min + t * (max - min);
    }

    bool Random::Bool(float32 probability)
    {
        return Float() < probability;
    }

    float32 Random::Gaussian(float32 mean, float32 stddev)
    {
        float32 u1 = Float(1e-7f, 1.0f);
        float32 u2 = Float(0.0f, 1.0f);
        float32 z  = std::sqrt(-2.0f * std::log(u1)) * std::cos(6.28318530718f * u2);
        return mean + stddev * z;
    }

} // namespace Good