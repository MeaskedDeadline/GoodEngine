#pragma once
#include "Core/Core.h"
#include <vector>

namespace Good
{

    struct LoadedImage
    {
        std::vector<byte> pixels;
        int32  width{0};
        int32  height{0};
        int32  channels{0};
        bool   valid{false};
    };

    class ImageLoader
    {
    public:
        static LoadedImage LoadFromFile(const std::string& path);
        static uint32 CreateTexture(const LoadedImage& image, bool smooth = true);
        static uint32 LoadTexture(const std::string& path, bool smooth = true);

    private:
        static LoadedImage LoadBMP(const std::string& path);
        static LoadedImage LoadTGA(const std::string& path);
        static LoadedImage LoadPNG(const std::string& path);
        static LoadedImage LoadJPEG(const std::string& path);

        static bool DecompressDeflate(const byte* input, usize inputSize,
                                       std::vector<byte>& output);
        static void PNGUnfilter(byte* pixels, int32 width, int32 height,
                                 int32 bytesPerPixel);
    };

} // namespace Good