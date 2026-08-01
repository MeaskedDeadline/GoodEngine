#pragma once
#include "Serialization/JsonValue.h"
#include "Scene/Scene.h"

namespace Good
{

    class SceneSerializer
    {
    public:
        static JsonValue ToJson(const Scene& scene);
        static bool SaveToFile(const Scene& scene, const std::string& path, bool pretty = true);
        static bool LoadFromFile(Scene& scene, const std::string& path);

    private:
        static JsonValue SerializeNode(const Ref<SceneNode>& node);
        static void      DeserializeNode(Scene& scene, const JsonValue& nodeVal,
                                         Ref<SceneNode> parent);
        static JsonValue SerializeVec3(const Vec3& v);
        static Vec3      DeserializeVec3(const JsonValue& v, const Vec3& def);
    };

} // namespace Good