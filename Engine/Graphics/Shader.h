#pragma once
#include "../Core/Core.h"
#include "../Math/Vec2.h"
#include "../Math/Vec3.h"
#include "../Math/Vec4.h"
#include "../Math/Mat4.h"
#include "../Math/Color.h"

namespace Good
{

    class Shader : public NonCopyable
    {
    public:
        Shader();
        ~Shader();

        bool LoadFromSource(const std::string& vertSrc, const std::string& fragSrc);
        bool LoadFromFiles(const std::string& vertPath, const std::string& fragPath);

        void Bind()   const;
        void Unbind() const;

        bool IsValid() const { return m_ID != 0; }
        uint32 GetID() const { return m_ID; }

        void SetInt(const std::string& name, int32 value);
        void SetFloat(const std::string& name, float32 value);
        void SetVec2(const std::string& name, const Vec2& value);
        void SetVec3(const std::string& name, const Vec3& value);
        void SetVec4(const std::string& name, const Vec4& value);
        void SetColor(const std::string& name, const Color& value);
        void SetMat4(const std::string& name, const Mat4& value);

    private:
        uint32 Compile(uint32 type, const std::string& source);
        int32  GetLocation(const std::string& name);

        uint32 m_ID{0};
        std::unordered_map<std::string, int32> m_Cache;
    };

} // namespace Good