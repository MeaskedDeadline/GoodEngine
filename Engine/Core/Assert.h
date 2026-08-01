#pragma once
#include "Core.h"
#include "Log.h"

namespace Good
{
    GOOD_NOINLINE void AssertFail(const char* expr, const char* msg, const char* file, int32 line);
}

#if defined(GOOD_DEBUG)
#define GOOD_ASSERT(expr, msg) do { if (!(expr)) ::Good::AssertFail(#expr, msg, __FILE__, __LINE__); } while(false)
#else
#define GOOD_ASSERT(expr, msg) ((void)(expr))
#endif