#pragma once
#include "../Core/Core.h"

namespace Good
{

    enum class AudioFormat : uint8 { Unknown, PCM8, PCM16, PCM32, Float32 };

    struct AudioClipInfo
    {
        uint32      sampleRate{44100};
        uint32      channels{2};
        uint32      sampleCount{0};
        float32     duration{0.0f};
        AudioFormat format{AudioFormat::PCM16};
    };

    class AudioClip
    {
    public:
        AudioClip() = default;
        ~AudioClip() = default;

        bool LoadFromFile(const std::string& path);

        const std::string& GetPath()  const { return m_Path;  }
        const AudioClipInfo& GetInfo()const { return m_Info;  }
        const std::vector<byte>& GetData() const { return m_Data; }

        bool IsLoaded() const { return m_Loaded; }

    private:
        bool LoadWAV(const std::string& path);

        std::string   m_Path;
        AudioClipInfo m_Info;
        std::vector<byte> m_Data;
        bool          m_Loaded{false};
    };

} // namespace Good