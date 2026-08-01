#include "Assert.h"

namespace Good
{
    GOOD_NOINLINE void AssertFail(const char* expr, const char* msg, const char* file, int32 line)
    {
        GOOD_LOG_FATAL("Assert", "Failed: {} | {} | {}:{}", expr, msg, file, line);
        GOOD_DEBUGBREAK();
    }
}