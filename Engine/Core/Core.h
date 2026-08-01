#pragma once

#include <cstdint>
#include <cstddef>
#include <memory>
#include <functional>
#include <string>
#include <string_view>
#include <vector>
#include <array>
#include <unordered_map>
#include <unordered_set>
#include <optional>
#include <variant>
#include <type_traits>
#include <concepts>
#include <algorithm>
#include <utility>
#include <cassert>

#if defined(_WIN32) || defined(_WIN64)
    #ifndef GOOD_PLATFORM_WINDOWS
        #define GOOD_PLATFORM_WINDOWS 1
    #endif
#elif defined(__linux__)
    #ifndef GOOD_PLATFORM_LINUX
        #define GOOD_PLATFORM_LINUX 1
    #endif
#elif defined(__APPLE__)
    #ifndef GOOD_PLATFORM_MACOS
        #define GOOD_PLATFORM_MACOS 1
    #endif
#endif

#if defined(_MSC_VER)
    #define GOOD_FORCEINLINE  __forceinline
    #define GOOD_NOINLINE     __declspec(noinline)
    #define GOOD_DEBUGBREAK() __debugbreak()
#elif defined(__GNUC__) || defined(__clang__)
    #define GOOD_FORCEINLINE  __attribute__((always_inline)) inline
    #define GOOD_NOINLINE     __attribute__((noinline))
    #define GOOD_DEBUGBREAK() __builtin_trap()
#else
    #define GOOD_FORCEINLINE  inline
    #define GOOD_NOINLINE
    #define GOOD_DEBUGBREAK() ((void)0)
#endif

#if defined(GOOD_PLATFORM_WINDOWS)
    #define GOOD_API __declspec(dllexport)
#else
    #define GOOD_API __attribute__((visibility("default")))
#endif

namespace Good
{
    using int8    = std::int8_t;
    using int16   = std::int16_t;
    using int32   = std::int32_t;
    using int64   = std::int64_t;
    using uint8   = std::uint8_t;
    using uint16  = std::uint16_t;
    using uint32  = std::uint32_t;
    using uint64  = std::uint64_t;
    using float32 = float;
    using float64 = double;
    using byte    = std::uint8_t;
    using usize   = std::size_t;
    using isize   = std::ptrdiff_t;

    template<typename T> using Ref     = std::shared_ptr<T>;
    template<typename T> using Scope   = std::unique_ptr<T>;
    template<typename T> using WeakRef = std::weak_ptr<T>;

    template<typename T, typename... Args>
    GOOD_FORCEINLINE Ref<T> MakeRef(Args&&... args)
    {
        return std::make_shared<T>(std::forward<Args>(args)...);
    }

    template<typename T, typename... Args>
    GOOD_FORCEINLINE Scope<T> MakeScope(Args&&... args)
    {
        return std::make_unique<T>(std::forward<Args>(args)...);
    }

    template<typename T>
    GOOD_FORCEINLINE constexpr T Bit(T x) { return static_cast<T>(1) << x; }

    struct NonCopyable
    {
        NonCopyable()                              = default;
        NonCopyable(const NonCopyable&)            = delete;
        NonCopyable& operator=(const NonCopyable&) = delete;
        NonCopyable(NonCopyable&&)                 = default;
        NonCopyable& operator=(NonCopyable&&)      = default;
    };
}