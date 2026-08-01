#include "UI/ImageLoader.h"
#include "Graphics/GLFunctions.h"
#include "Core/Log.h"
#include "Utilities/StringUtils.h"
#include <fstream>
#include <cstring>
#include <cmath>
#include <algorithm>

namespace Good
{

LoadedImage ImageLoader::LoadFromFile(const std::string& path)
{
    usize dotPos = path.find_last_of('.');
    if (dotPos == std::string::npos) { GOOD_LOG_ERROR("ImageLoader", "No extension '{}'", path); return {}; }

    std::string ext = StringUtils::ToLower(path.substr(dotPos + 1));

    if (ext == "bmp")                    return LoadBMP(path);
    if (ext == "tga")                    return LoadTGA(path);
    if (ext == "png")                    return LoadPNG(path);
    if (ext == "jpg" || ext == "jpeg")   return LoadJPEG(path);

    GOOD_LOG_ERROR("ImageLoader", "Unsupported format '.{}'", ext);
    return {};
}

uint32 ImageLoader::CreateTexture(const LoadedImage& image, bool smooth)
{
    if (!image.valid || image.pixels.empty()) return 0;

    GLenum format = GL_RGBA;
    if (image.channels == 3) format = GL_RGB;

    uint32 texID = 0;
    glGenTextures(1, &texID);
    glBindTexture(GL_TEXTURE_2D, texID);

    GLint filter = smooth ? GL_LINEAR : GL_NEAREST;
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, filter);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, filter);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA,
        image.width, image.height, 0,
        format, GL_UNSIGNED_BYTE, image.pixels.data());

    glBindTexture(GL_TEXTURE_2D, 0);

    GOOD_LOG_INFO("ImageLoader", "Texture {}x{} ch={} id={}", image.width, image.height, image.channels, texID);
    return texID;
}

uint32 ImageLoader::LoadTexture(const std::string& path, bool smooth)
{
    auto image = LoadFromFile(path);
    if (!image.valid) return 0;
    return CreateTexture(image, smooth);
}

// ═══════════════════════════════════════════════════════════════════
// BMP
// ═══════════════════════════════════════════════════════════════════

#pragma pack(push, 1)
struct BMPHeader { uint16 sig; uint32 fileSize; uint16 r1,r2; uint32 dataOffset; uint32 headerSize; int32 width,height; uint16 planes,bpp; uint32 compression; };
#pragma pack(pop)

LoadedImage ImageLoader::LoadBMP(const std::string& path)
{
    std::ifstream file(path, std::ios::binary);
    if (!file.is_open()) { GOOD_LOG_ERROR("ImageLoader", "Cannot open BMP '{}'", path); return {}; }

    BMPHeader hdr{}; file.read(reinterpret_cast<char*>(&hdr), sizeof(hdr));
    if (hdr.sig != 0x4D42 || hdr.compression != 0) { GOOD_LOG_ERROR("ImageLoader", "Invalid BMP '{}'", path); return {}; }

    bool topDown = hdr.height < 0;
    int32 w = hdr.width, h = topDown ? -hdr.height : hdr.height, ch = hdr.bpp / 8;
    int32 rowStride = (w * ch + 3) & ~3;

    file.seekg(hdr.dataOffset);
    std::vector<byte> raw(rowStride * h); file.read(reinterpret_cast<char*>(raw.data()), raw.size());

    LoadedImage img; img.width = w; img.height = h; img.channels = 4; img.pixels.resize(w * h * 4); img.valid = true;

    for (int32 y = 0; y < h; ++y)
    {
        int32 srcY = topDown ? y : (h - 1 - y);
        const byte* s = raw.data() + srcY * rowStride;
        byte* d = img.pixels.data() + y * w * 4;
        for (int32 x = 0; x < w; ++x)
        {
            d[x*4]   = s[x*ch+2]; d[x*4+1] = s[x*ch+1]; d[x*4+2] = s[x*ch];
            d[x*4+3] = (ch == 4) ? s[x*ch+3] : 255;
        }
    }

    GOOD_LOG_INFO("ImageLoader", "BMP '{}' {}x{}", path, w, h);
    return img;
}

// ═══════════════════════════════════════════════════════════════════
// TGA
// ═══════════════════════════════════════════════════════════════════

LoadedImage ImageLoader::LoadTGA(const std::string& path)
{
    std::ifstream file(path, std::ios::binary);
    if (!file.is_open()) { GOOD_LOG_ERROR("ImageLoader", "Cannot open TGA '{}'", path); return {}; }

    byte header[18]; file.read(reinterpret_cast<char*>(header), 18);
    int32 w = header[12]|(header[13]<<8), h = header[14]|(header[15]<<8), ch = header[16]/8;
    if (ch != 3 && ch != 4) { GOOD_LOG_ERROR("ImageLoader", "TGA bpp {} '{}'", header[16], path); return {}; }

    if (header[0] > 0) file.seekg(18 + header[0]);
    std::vector<byte> raw(w * h * ch); file.read(reinterpret_cast<char*>(raw.data()), raw.size());
    bool topDown = (header[17] & 0x20) != 0;

    LoadedImage img; img.width = w; img.height = h; img.channels = 4; img.pixels.resize(w * h * 4); img.valid = true;

    for (int32 y = 0; y < h; ++y)
    {
        int32 srcY = topDown ? y : (h - 1 - y);
        const byte* s = raw.data() + srcY * w * ch;
        byte* d = img.pixels.data() + y * w * 4;
        for (int32 x = 0; x < w; ++x)
        {
            d[x*4] = s[x*ch+2]; d[x*4+1] = s[x*ch+1]; d[x*4+2] = s[x*ch];
            d[x*4+3] = (ch == 4) ? s[x*ch+3] : 255;
        }
    }

    GOOD_LOG_INFO("ImageLoader", "TGA '{}' {}x{}", path, w, h);
    return img;
}

// ═══════════════════════════════════════════════════════════════════
// PNG - полный DEFLATE
// ═══════════════════════════════════════════════════════════════════

static uint32 PNGReadU32(const byte* p)
{
    return (uint32(p[0])<<24)|(uint32(p[1])<<16)|(uint32(p[2])<<8)|p[3];
}

namespace {
    struct HTree {
        int32 counts[16]={}, symbols[320]={};
        void Build(const int32* lens, int32 n) {
            int32 off[16]={}; for(int32 i=0;i<16;i++) counts[i]=0;
            for(int32 i=0;i<n;i++) if(lens[i]) counts[lens[i]]++;
            off[1]=0; for(int32 i=1;i<15;i++) off[i+1]=off[i]+counts[i];
            for(int32 i=0;i<n;i++) if(lens[i]) symbols[off[lens[i]]++]=i;
        }
        
        template<typename F>
        int32 Decode(F& rb) const {
            int32 code=0,first=0,idx=0;
            for(int32 len=1;len<=15;len++){
                code|=rb(); int32 c=counts[len];
                if(code-c<first) return symbols[idx+(code-first)];
                idx+=c; first=(first+c)<<1; code<<=1;
            } return -1;
        }
    };
}

bool ImageLoader::DecompressDeflate(const byte* input, usize inputSize, std::vector<byte>& output)
{
    if (inputSize < 6) return false;
    if ((input[0] & 0x0F) != 8) return false;

    const byte* src = input + 2;
    usize srcSize = inputSize - 6;
    output.clear(); output.reserve(inputSize * 4);

    usize pos = 0; int32 bitBuf = 0, bitCnt = 0;

    auto readBit = [&]() -> int32 {
        if (bitCnt == 0) { if (pos >= srcSize) return 0; bitBuf = src[pos++]; bitCnt = 8; }
        int32 b = bitBuf & 1; bitBuf >>= 1; --bitCnt; return b;
    };
    auto readBits = [&](int32 n) -> int32 { int32 v = 0; for (int32 i = 0; i < n; ++i) v |= readBit() << i; return v; };

    static const int32 lBase[29]={3,4,5,6,7,8,9,10,11,13,15,17,19,23,27,31,35,43,51,59,67,83,99,115,131,163,195,227,258};
    static const int32 lExtra[29]={0,0,0,0,0,0,0,0,1,1,1,1,2,2,2,2,3,3,3,3,4,4,4,4,5,5,5,5,0};
    static const int32 dBase[30]={1,2,3,4,5,7,9,13,17,25,33,49,65,97,129,193,257,385,513,769,1025,1537,2049,3073,4097,6145,8193,12289,16385,24577};
    static const int32 dExtra[30]={0,0,0,0,1,1,2,2,3,3,4,4,5,5,6,6,7,7,8,8,9,9,10,10,11,11,12,12,13,13};

    bool last = false;
    while (!last) {
        last = readBit(); int32 btype = readBits(2);
        if (btype == 0) {
            bitBuf=0; bitCnt=0; if(pos+4>srcSize) return false;
            uint16 len=src[pos]|(src[pos+1]<<8); pos+=4;
            for(uint16 i=0;i<len&&pos<srcSize;i++) output.push_back(src[pos++]);
        } else if (btype==1||btype==2) {
            HTree lit,dst;
            if(btype==1){
                int32 ll[288]; for(int32 i=0;i<144;i++)ll[i]=8; for(int32 i=144;i<256;i++)ll[i]=9;
                for(int32 i=256;i<280;i++)ll[i]=7; for(int32 i=280;i<288;i++)ll[i]=8;
                lit.Build(ll,288); int32 dl[32]; for(int32 i=0;i<32;i++)dl[i]=5; dst.Build(dl,32);
            } else {
                int32 hl=readBits(5)+257,hd=readBits(5)+1,hc=readBits(4)+4;
                static const int32 co[19]={16,17,18,0,8,7,9,6,10,5,11,4,12,3,13,2,14,1,15};
                int32 cl[19]={}; for(int32 i=0;i<hc;i++) cl[co[i]]=readBits(3);
                HTree ct; ct.Build(cl,19);
                int32 al[320]={}; int32 tot=hl+hd,idx=0;
                while(idx<tot){
                    int32 s=ct.Decode(readBit);
                    if(s>=0&&s<=15) al[idx++]=s;
                    else if(s==16){int32 r=readBits(2)+3,p=idx>0?al[idx-1]:0; for(int32 i=0;i<r&&idx<tot;i++)al[idx++]=p;}
                    else if(s==17){int32 r=readBits(3)+3; for(int32 i=0;i<r&&idx<tot;i++)al[idx++]=0;}
                    else if(s==18){int32 r=readBits(7)+11; for(int32 i=0;i<r&&idx<tot;i++)al[idx++]=0;}
                }
                lit.Build(al,hl); dst.Build(al+hl,hd);
            }
            while(true){
                int32 s=lit.Decode(readBit); if(s<0)return false;
                if(s<256) output.push_back((byte)s);
                else if(s==256) break;
                else{
                    int32 li=s-257; if(li<0||li>=29)return false;
                    int32 len=lBase[li]+readBits(lExtra[li]);
                    int32 ds=dst.Decode(readBit); if(ds<0||ds>=30)return false;
                    int32 dist=dBase[ds]+readBits(dExtra[ds]);
                    usize sp=output.size()-dist;
                    for(int32 i=0;i<len;i++) output.push_back(output[sp+i]);
                }
            }
        } else return false;
    }
    return true;
}

void ImageLoader::PNGUnfilter(byte* px, int32 w, int32 h, int32 bpp)
{
    int32 stride = w * bpp;
    for (int32 y = 0; y < h; ++y) {
        byte* row = px + y * (stride + 1); byte ft = row[0]; byte* d = row + 1;
        byte* prev = (y > 0) ? (px + (y-1) * (stride+1) + 1) : nullptr;
        switch (ft) {
            case 0: break;
            case 1: for(int32 x=bpp;x<stride;x++) d[x]+=d[x-bpp]; break;
            case 2: if(prev) for(int32 x=0;x<stride;x++) d[x]+=prev[x]; break;
            case 3: for(int32 x=0;x<stride;x++){int32 a=(x>=bpp)?d[x-bpp]:0;int32 b=prev?prev[x]:0;d[x]+=(byte)((a+b)/2);} break;
            case 4: for(int32 x=0;x<stride;x++){
                int32 a=(x>=bpp)?d[x-bpp]:0,b=prev?prev[x]:0,c=(prev&&x>=bpp)?prev[x-bpp]:0;
                int32 p=a+b-c,pa=std::abs(p-a),pb=std::abs(p-b),pc=std::abs(p-c);
                d[x]+=(byte)((pa<=pb&&pa<=pc)?a:(pb<=pc)?b:c);
            } break;
        }
    }
}

LoadedImage ImageLoader::LoadPNG(const std::string& path)
{
    std::ifstream file(path, std::ios::binary | std::ios::ate);
    if (!file.is_open()) { GOOD_LOG_ERROR("ImageLoader", "Cannot open PNG '{}'", path); return {}; }
    auto fs = file.tellg(); file.seekg(0);
    std::vector<byte> fd(static_cast<usize>(fs)); file.read(reinterpret_cast<char*>(fd.data()), fs);

    const byte sig[8]={137,80,78,71,13,10,26,10};
    if (std::memcmp(fd.data(),sig,8)!=0) { GOOD_LOG_ERROR("ImageLoader","Invalid PNG sig '{}'",path); return {}; }

    int32 w=0,h=0,bd=0,ct=0; std::vector<byte> idat;
    usize p=8;
    while(p+12<=fd.size()){
        uint32 cl=PNGReadU32(&fd[p]); char tp[5]={}; std::memcpy(tp,&fd[p+4],4);
        const byte* cd=&fd[p+8];
        if(!std::strcmp(tp,"IHDR")&&cl>=13){w=(int32)PNGReadU32(cd);h=(int32)PNGReadU32(cd+4);bd=cd[8];ct=cd[9];}
        else if(!std::strcmp(tp,"IDAT")) idat.insert(idat.end(),cd,cd+cl);
        else if(!std::strcmp(tp,"IEND")) break;
        p+=12+cl;
    }

    if(w==0||h==0||bd!=8){GOOD_LOG_ERROR("ImageLoader","Unsupported PNG '{}'",path);return {};}
    int32 ch=0;
    switch(ct){case 0:ch=1;break;case 2:ch=3;break;case 4:ch=2;break;case 6:ch=4;break;
        default:GOOD_LOG_ERROR("ImageLoader","PNG color type {} '{}'",ct,path);return {};}

    std::vector<byte> dec;
    if(!DecompressDeflate(idat.data(),idat.size(),dec)){GOOD_LOG_ERROR("ImageLoader","PNG deflate fail '{}'",path);return {};}

    int32 stride=w*ch, expected=h*(stride+1);
    if((int32)dec.size()<expected){GOOD_LOG_ERROR("ImageLoader","PNG size mismatch '{}'",path);return {};}

    PNGUnfilter(dec.data(),w,h,ch);

    LoadedImage img; img.width=w; img.height=h; img.channels=4; img.pixels.resize(w*h*4); img.valid=true;
    for(int32 y=0;y<h;y++){
        const byte* sr=dec.data()+y*(stride+1)+1; byte* dr=img.pixels.data()+y*w*4;
        for(int32 x=0;x<w;x++){
            const byte* s=sr+x*ch; byte* d=dr+x*4;
            switch(ch){
                case 1:d[0]=d[1]=d[2]=s[0];d[3]=255;break;
                case 2:d[0]=d[1]=d[2]=s[0];d[3]=s[1];break;
                case 3:d[0]=s[0];d[1]=s[1];d[2]=s[2];d[3]=255;break;
                case 4:d[0]=s[0];d[1]=s[1];d[2]=s[2];d[3]=s[3];break;
            }
        }
    }

    GOOD_LOG_INFO("ImageLoader","PNG '{}' {}x{} ch={}",path,w,h,ch);
    return img;
}

// ═══════════════════════════════════════════════════════════════════
// JPEG - базовый baseline декодер
// ═══════════════════════════════════════════════════════════════════

LoadedImage ImageLoader::LoadJPEG(const std::string& path)
{
    std::ifstream file(path, std::ios::binary | std::ios::ate);
    if (!file.is_open()) { GOOD_LOG_ERROR("ImageLoader", "Cannot open JPEG '{}'", path); return {}; }
    auto fs = file.tellg(); file.seekg(0);
    std::vector<byte> data(static_cast<usize>(fs));
    file.read(reinterpret_cast<char*>(data.data()), fs);

    if (data.size() < 4 || data[0] != 0xFF || data[1] != 0xD8)
    {
        GOOD_LOG_ERROR("ImageLoader", "Invalid JPEG '{}'", path);
        return {};
    }

    // JPEG маркеры
    int32 width = 0, height = 0, numComponents = 0;

    // Таблицы квантования
    int32 qtables[4][64] = {};
    bool qtableSet[4] = {};

    // Huffman таблицы
    struct HuffTable
    {
        byte bits[16] = {};
        byte values[256] = {};
        int32 numValues = 0;

        // Для быстрого декодирования
        int32 maxCode[16] = {};
        int32 minCode[16] = {};
        int32 valOffset[16] = {};

        void Build()
        {
            int32 code = 0, idx = 0;
            for (int32 i = 0; i < 16; ++i)
            {
                minCode[i] = code;
                maxCode[i] = code + bits[i] - 1;
                valOffset[i] = idx - code;
                code += bits[i];
                idx += bits[i];
                code <<= 1;
            }
        }
    };

    HuffTable dcTables[4], acTables[4];

    // Компоненты
    struct Component
    {
        int32 id = 0, hSamp = 1, vSamp = 1;
        int32 qtableIdx = 0;
        int32 dcTableIdx = 0, acTableIdx = 0;
        int32 dcPred = 0;
    };

    Component components[4];

    // Зигзаг
    static const int32 zigzag[64] = {
        0,1,8,16,9,2,3,10,17,24,32,25,18,11,4,5,
        12,19,26,33,40,48,41,34,27,20,13,6,7,14,21,28,
        35,42,49,56,57,50,43,36,29,22,15,23,30,37,44,51,
        58,59,52,45,38,31,39,46,53,60,61,54,47,55,62,63
    };

    // Парсим маркеры
    usize pos = 2;

    while (pos < data.size() - 1)
    {
        if (data[pos] != 0xFF) { ++pos; continue; }

        byte marker = data[pos + 1];
        pos += 2;

        if (marker == 0xD9) break; // EOI

        if (marker >= 0xD0 && marker <= 0xD7) continue; // RST
        if (marker == 0x00 || marker == 0x01) continue;

        if (pos + 2 > data.size()) break;
        uint16 segLen = (data[pos] << 8) | data[pos + 1];
        const byte* seg = &data[pos + 2];

        // DQT - квантование
        if (marker == 0xDB)
        {
            usize off = 0;
            while (off < segLen - 2)
            {
                int32 prec = (seg[off] >> 4) & 0x0F;
                int32 tidx = seg[off] & 0x0F;
                ++off;
                if (tidx < 4)
                {
                    for (int32 i = 0; i < 64; ++i)
                    {
                        if (prec == 0)
                            qtables[tidx][i] = seg[off++];
                        else
                        {
                            qtables[tidx][i] = (seg[off] << 8) | seg[off + 1];
                            off += 2;
                        }
                    }
                    qtableSet[tidx] = true;
                }
            }
        }

        // SOF0 - baseline
        if (marker == 0xC0)
        {
            height = (seg[1] << 8) | seg[2];
            width  = (seg[3] << 8) | seg[4];
            numComponents = seg[5];

            for (int32 i = 0; i < numComponents && i < 4; ++i)
            {
                components[i].id     = seg[6 + i * 3];
                components[i].hSamp  = (seg[7 + i * 3] >> 4) & 0x0F;
                components[i].vSamp  = seg[7 + i * 3] & 0x0F;
                components[i].qtableIdx = seg[8 + i * 3];
            }
        }

        // DHT - Huffman
        if (marker == 0xC4)
        {
            usize off = 0;
            while (off < segLen - 2)
            {
                int32 cls  = (seg[off] >> 4) & 0x0F; // 0=DC, 1=AC
                int32 tidx = seg[off] & 0x0F;
                ++off;

                HuffTable* ht = (cls == 0) ? &dcTables[tidx] : &acTables[tidx];
                ht->numValues = 0;

                for (int32 i = 0; i < 16; ++i)
                {
                    ht->bits[i] = seg[off++];
                    ht->numValues += ht->bits[i];
                }

                for (int32 i = 0; i < ht->numValues; ++i)
                    ht->values[i] = seg[off++];

                ht->Build();
            }
        }

        // SOS - scan data
        if (marker == 0xDA)
        {
            int32 ns = seg[0];
            for (int32 i = 0; i < ns && i < 4; ++i)
            {
                components[i].dcTableIdx = (seg[2 + i * 2] >> 4) & 0x0F;
                components[i].acTableIdx = seg[2 + i * 2] & 0x0F;
                components[i].dcPred = 0;
            }

            usize scanStart = pos + segLen;

            // Собираем scan data (убираем byte stuffing)
            std::vector<byte> scanData;
            for (usize i = scanStart; i < data.size() - 1; ++i)
            {
                if (data[i] == 0xFF)
                {
                    if (data[i + 1] == 0x00)
                    {
                        scanData.push_back(0xFF);
                        ++i;
                    }
                    else if (data[i + 1] >= 0xD0 && data[i + 1] <= 0xD7)
                    {
                        ++i; // skip RST
                    }
                    else break;
                }
                else
                {
                    scanData.push_back(data[i]);
                }
            }

            // Декодируем MCU
            if (width == 0 || height == 0 || numComponents == 0) break;

            LoadedImage img;
            img.width    = width;
            img.height   = height;
            img.channels = 3;
            img.pixels.resize(width * height * 3, 128);
            img.valid    = true;

            int32 bitPos = 0;

            auto getBit = [&]() -> int32
            {
                int32 byteIdx = bitPos / 8;
                int32 bitIdx  = 7 - (bitPos % 8);
                ++bitPos;
                if (byteIdx >= (int32)scanData.size()) return 0;
                return (scanData[byteIdx] >> bitIdx) & 1;
            };

            auto getBits = [&](int32 n) -> int32
            {
                int32 val = 0;
                for (int32 i = 0; i < n; ++i)
                    val = (val << 1) | getBit();
                return val;
            };

            auto decodeHuff = [&](const HuffTable& ht) -> int32
            {
                int32 code = 0;
                for (int32 i = 0; i < 16; ++i)
                {
                    code = (code << 1) | getBit();
                    if (ht.bits[i] > 0 && code <= ht.maxCode[i])
                        return ht.values[code + ht.valOffset[i]];
                }
                return 0;
            };

            auto decodeValue = [&](int32 bits) -> int32
            {
                if (bits == 0) return 0;
                int32 val = getBits(bits);
                if (val < (1 << (bits - 1)))
                    val -= (1 << bits) - 1;
                return val;
            };

            // Простой декодер — только 4:4:4 без субсемплинга
            int32 mcuW = 8, mcuH = 8;
            int32 mcuCols = (width + mcuW - 1) / mcuW;
            int32 mcuRows = (height + mcuH - 1) / mcuH;

            for (int32 my = 0; my < mcuRows; ++my)
            {
                for (int32 mx = 0; mx < mcuCols; ++mx)
                {
                    int32 blocks[3][64] = {};

                    for (int32 comp = 0; comp < numComponents; ++comp)
                    {
                        int32 block[64] = {};

                        // DC
                        int32 dcLen = decodeHuff(dcTables[components[comp].dcTableIdx]);
                        int32 dcVal = decodeValue(dcLen);
                        components[comp].dcPred += dcVal;
                        block[0] = components[comp].dcPred;

                        // AC
                        int32 idx = 1;
                        while (idx < 64)
                        {
                            int32 ac = decodeHuff(acTables[components[comp].acTableIdx]);
                            if (ac == 0x00) break; // EOB
                            int32 zeros = (ac >> 4) & 0x0F;
                            int32 acLen = ac & 0x0F;
                            idx += zeros;
                            if (idx >= 64) break;
                            block[idx] = decodeValue(acLen);
                            ++idx;
                        }

                        // Dequantize
                        int32 qi = components[comp].qtableIdx;
                        for (int32 i = 0; i < 64; ++i)
                            block[zigzag[i]] *= qtables[qi][i];

                        // IDCT (простая)
                        float32 tmp[64], out[64];

                        for (int32 i = 0; i < 64; ++i)
                            tmp[i] = static_cast<float32>(block[i]);

                        // Row IDCT
                        for (int32 row = 0; row < 8; ++row)
                        {
                            float32* r = &tmp[row * 8];
                            for (int32 x = 0; x < 8; ++x)
                            {
                                float32 sum = 0;
                                for (int32 u = 0; u < 8; ++u)
                                {
                                    float32 cu = (u == 0) ? 0.353553f : 0.5f;
                                    sum += cu * r[u] * std::cos((2.0f * x + 1.0f) * u * 3.14159265f / 16.0f);
                                }
                                out[row * 8 + x] = sum;
                            }
                        }

                        // Col IDCT
                        for (int32 col = 0; col < 8; ++col)
                        {
                            for (int32 y = 0; y < 8; ++y)
                            {
                                float32 sum = 0;
                                for (int32 v = 0; v < 8; ++v)
                                {
                                    float32 cv = (v == 0) ? 0.353553f : 0.5f;
                                    sum += cv * out[v * 8 + col] * std::cos((2.0f * y + 1.0f) * v * 3.14159265f / 16.0f);
                                }
                                blocks[comp][y * 8 + col] = static_cast<int32>(sum + 128.5f);
                            }
                        }
                    }

                    // YCbCr → RGB и записываем
                    for (int32 by = 0; by < 8; ++by)
                    {
                        for (int32 bx = 0; bx < 8; ++bx)
                        {
                            int32 px = mx * 8 + bx;
                            int32 py = my * 8 + by;
                            if (px >= width || py >= height) continue;

                            int32 pidx = (py * width + px) * 3;

                            if (numComponents == 1)
                            {
                                int32 v = std::clamp(blocks[0][by * 8 + bx], 0, 255);
                                img.pixels[pidx] = img.pixels[pidx+1] = img.pixels[pidx+2] = (byte)v;
                            }
                            else
                            {
                                float32 Y  = static_cast<float32>(blocks[0][by * 8 + bx]);
                                float32 Cb = static_cast<float32>(blocks[1][by * 8 + bx]) - 128.0f;
                                float32 Cr = static_cast<float32>(blocks[2][by * 8 + bx]) - 128.0f;

                                int32 R = static_cast<int32>(Y + 1.402f * Cr);
                                int32 G = static_cast<int32>(Y - 0.344136f * Cb - 0.714136f * Cr);
                                int32 B = static_cast<int32>(Y + 1.772f * Cb);

                                img.pixels[pidx]   = (byte)std::clamp(R, 0, 255);
                                img.pixels[pidx+1] = (byte)std::clamp(G, 0, 255);
                                img.pixels[pidx+2] = (byte)std::clamp(B, 0, 255);
                            }
                        }
                    }
                }
            }

            GOOD_LOG_INFO("ImageLoader", "JPEG '{}' {}x{}", path, width, height);
            return img;
        }

        pos += segLen;
    }

    GOOD_LOG_ERROR("ImageLoader", "JPEG decode failed '{}'", path);
    return {};
}

} // namespace Good
