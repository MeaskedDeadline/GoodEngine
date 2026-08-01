#include "Asset.h"

namespace Good
{

    const char* AssetTypeToString(AssetType t)
    {
        switch (t)
        {
        case AssetType::Texture:       return "Texture";
        case AssetType::Mesh:          return "Mesh";
        case AssetType::Shader:        return "Shader";
        case AssetType::Material:      return "Material";
        case AssetType::AudioClip:     return "AudioClip";
        case AssetType::AnimationClip: return "AnimationClip";
        case AssetType::Scene:         return "Scene";
        case AssetType::Prefab:        return "Prefab";
        case AssetType::Font:          return "Font";
        case AssetType::Script:        return "Script";
        default:                       return "Unknown";
        }
    }

    AssetType AssetTypeFromExtension(const std::string& ext)
    {
        if (ext==".png"||ext==".jpg"||ext==".bmp"||ext==".tga") return AssetType::Texture;
        if (ext==".obj"||ext==".fbx"||ext==".gltf"||ext==".mesh") return AssetType::Mesh;
        if (ext==".glsl"||ext==".vert"||ext==".frag")             return AssetType::Shader;
        if (ext==".mat")                                          return AssetType::Material;
        if (ext==".wav"||ext==".ogg"||ext==".mp3")                return AssetType::AudioClip;
        if (ext==".anim")                                         return AssetType::AnimationClip;
        if (ext==".scene"||ext==".json")                          return AssetType::Scene;
        if (ext==".prefab")                                       return AssetType::Prefab;
        if (ext==".fnt"||ext==".ttf")                             return AssetType::Font;
        if (ext==".cpp"||ext==".lua"||ext==".py")                 return AssetType::Script;
        return AssetType::Unknown;
    }

} // namespace Good