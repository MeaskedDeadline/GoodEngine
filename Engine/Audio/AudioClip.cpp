#include "AudioClip.h"
#include "../Core/Log.h"
#include <fstream>

namespace Good
{

#pragma pack(push, 1)
struct WAVHeader
{
    char     riff[4];
    uint32   chunkSize;
    char     wave[4];
    char     fmt[4];
    uint32   fmtSize;
    uint16   audioFormat;
    uint16   channels;
    uint32   sampleRate;
    uint32   byteRate;
    uint16   blockAlign;
    uint16   bitsPerSample;
    char     data[4];
    uint32   dataSize;
};
#pragma pack(pop)

bool AudioClip::LoadFromFile(const std::string& path)
{
    std::string lower = path;
    for (auto& c : lower) c = static_cast<char>(std::tolower(static_cast<unsigned char>(c)));

    if (lower.size() >= 4 && lower.substr(lower.size()-4) == ".wav")
        return LoadWAV(path);

    GOOD_LOG_ERROR("AudioClip", "Unsupported format: '{}'", path);
    return false;
}

bool AudioClip::LoadWAV(const std::string& path)
{
    std::ifstream file(path, std::ios::binary);
    if (!file.is_open())
    {
        GOOD_LOG_ERROR("AudioClip", "Cannot open: '{}'", path);
        return false;
    }

    WAVHeader header{};
    file.read(reinterpret_cast<char*>(&header), sizeof(header));

    if (std::string(header.riff, 4) != "RIFF" ||
        std::string(header.wave, 4) != "WAVE")
    {
        GOOD_LOG_ERROR("AudioClip", "Invalid WAV: '{}'", path);
        return false;
    }

    m_Info.sampleRate   = header.sampleRate;
    m_Info.channels     = header.channels;
    m_Info.format       = (header.bitsPerSample == 8)  ? AudioFormat::PCM8
                        : (header.bitsPerSample == 16) ? AudioFormat::PCM16
                        :                                AudioFormat::PCM32;
    m_Info.sampleCount  = header.dataSize / (header.bitsPerSample / 8);
    m_Info.duration     = static_cast<float32>(m_Info.sampleCount) /
                          static_cast<float32>(header.sampleRate * header.channels);

    m_Data.resize(header.dataSize);
    file.read(reinterpret_cast<char*>(m_Data.data()),
              static_cast<std::streamsize>(header.dataSize));

    m_Path   = path;
    m_Loaded = true;

    GOOD_LOG_INFO("AudioClip", "Loaded '{}' ({}Hz, {}ch, {:.2f}s)",
        path, m_Info.sampleRate, m_Info.channels, m_Info.duration);
    return true;
}

} // namespace Good