#pragma once
#include "../Core/Core.h"

namespace Good
{

    class Random
    {
    public:
        static void    Seed(uint64 seed);
        static int32   Int(int32 min, int32 max);
        static float32 Float(float32 min = 0.0f, float32 max = 1.0f);
        static float64 Double(float64 min = 0.0, float64 max = 1.0);
        static bool    Bool(float32 probability = 0.5f);

        static float32 Gaussian(float32 mean = 0.0f, float32 stddev = 1.0f);

    private:
        static uint64 s_State;
        static uint64 Next();
    };

} // namespace Good