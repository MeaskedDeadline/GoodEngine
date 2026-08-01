#include "Core.h"

namespace Good
{
    static_assert(sizeof(int8)    == 1);
    static_assert(sizeof(int16)   == 2);
    static_assert(sizeof(int32)   == 4);
    static_assert(sizeof(int64)   == 8);
    static_assert(sizeof(float32) == 4);
    static_assert(sizeof(float64) == 8);
}