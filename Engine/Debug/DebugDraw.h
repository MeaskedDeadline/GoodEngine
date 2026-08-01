#pragma once
#include "../Core/Core.h"
#include "../Math/Vec3.h"
#include "../Math/Color.h"
#include "../Math/Mat4.h"


namespace Good
{

    class DebugDraw
    {
    public:
        static void Line(const Vec3& from, const Vec3& to,
                         const Color& color = Color::Green(),
                         float32 duration = 0.0f);

        static void Ray(const Vec3& origin, const Vec3& direction, float32 length = 1.0f,
                        const Color& color = Color::Green(), float32 duration = 0.0f);

        static void Sphere(const Vec3& center, float32 radius,
                           const Color& color = Color::Green(), float32 duration = 0.0f);

        static void Box(const Vec3& center, const Vec3& size,
                        const Color& color = Color::Green(), float32 duration = 0.0f);

        static void Axes(const Mat4& transform, float32 size = 1.0f, float32 duration = 0.0f);

        static void Cross(const Vec3& position, float32 size = 0.1f,
                          const Color& color = Color::White(), float32 duration = 0.0f);

        static void Grid(const Vec3& center, float32 size = 10.0f, int32 divisions = 10,
                         const Color& color = Color(0.3f, 0.3f, 0.3f, 1.0f));

        static void Update(float32 dt);
        static void Flush(const Mat4& viewProjection);
        static void Clear();
        static void InitGL();
        static uint32 s_LineVAO;
        static uint32 s_LineVBO;
        static uint32 s_LineShader;
        static bool   s_GLInited;

    private:
        struct DebugLine
        {
            Vec3    from, to;
            Color   color;
            float32 remainingTime{0.0f};
        };

        static std::vector<DebugLine> s_Lines;
    };

} // namespace Good