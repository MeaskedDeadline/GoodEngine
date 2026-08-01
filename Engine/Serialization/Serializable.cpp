#include "Serialization/SceneSerializer.h"
#include "Serialization/JsonParser.h"
#include "Serialization/JsonWriter.h"
#include "Core/Log.h"

namespace Good
{

JsonValue SceneSerializer::ToJson(const Scene& scene)
{
    JsonValue root = JsonValue::MakeObject();
    root["name"]   = scene.GetName();

    JsonValue nodes = JsonValue::MakeArray();
    for (const auto& child : scene.GetRoot()->GetChildren())
        nodes.PushBack(SerializeNode(child));

    root["nodes"] = std::move(nodes);
    return root;
}

bool SceneSerializer::SaveToFile(const Scene& scene, const std::string& path, bool pretty)
{
    JsonValue root = ToJson(scene);
    if (!JsonWriter::WriteToFile(root, path, pretty))
    {
        GOOD_LOG_ERROR("SceneSerializer", "Failed to save '{}'", path);
        return false;
    }
    GOOD_LOG_INFO("SceneSerializer", "Saved '{}'", path);
    return true;
}

bool SceneSerializer::LoadFromFile(Scene& scene, const std::string& path)
{
    JsonValue root;
    std::string error;
    if (!JsonParser::ParseFile(path, root, &error))
    {
        GOOD_LOG_ERROR("SceneSerializer", "Parse failed '{}': {}", path, error);
        return false;
    }

    scene.Clear();
    if (root.Contains("name")) scene.SetName(root["name"].AsString());

    const JsonValue& nodes = root["nodes"];
    if (nodes.IsArray())
        for (const JsonValue& nv : nodes.AsArray())
            DeserializeNode(scene, nv, scene.GetRoot());

    GOOD_LOG_INFO("SceneSerializer", "Loaded '{}'", path);
    return true;
}

JsonValue SceneSerializer::SerializeNode(const Ref<SceneNode>& node)
{
    JsonValue obj = JsonValue::MakeObject();
    obj["name"]     = node->GetName();
    obj["active"]   = node->IsActive();
    const Transform& tr = node->GetLocalTransform();
    obj["position"] = SerializeVec3(tr.position);
    obj["rotation"] = SerializeVec3(tr.rotation.ToEuler());
    obj["scale"]    = SerializeVec3(tr.scale);

    JsonValue children = JsonValue::MakeArray();
    for (const auto& child : node->GetChildren())
        children.PushBack(SerializeNode(child));
    obj["children"] = std::move(children);

    return obj;
}

void SceneSerializer::DeserializeNode(Scene& scene, const JsonValue& nv,
                                      Ref<SceneNode> parent)
{
    std::string name = nv["name"].AsString();
    if (name.empty()) name = "Node";

    Ref<SceneNode> node = scene.CreateNode(name, parent);
    node->SetActive(nv["active"].AsBool(true));

    Transform& tr = node->GetLocalTransform();
    tr.position   = DeserializeVec3(nv["position"], Vec3(0,0,0));
    tr.rotation   = Quat::FromEuler(DeserializeVec3(nv["rotation"], Vec3(0,0,0)));
    tr.scale      = DeserializeVec3(nv["scale"],    Vec3(1,1,1));

    const JsonValue& children = nv["children"];
    if (children.IsArray())
        for (const JsonValue& cv : children.AsArray())
            DeserializeNode(scene, cv, node);
}

JsonValue SceneSerializer::SerializeVec3(const Vec3& v)
{
    JsonValue arr = JsonValue::MakeArray();
    arr.PushBack(v.x);
    arr.PushBack(v.y);
    arr.PushBack(v.z);
    return arr;
}

Vec3 SceneSerializer::DeserializeVec3(const JsonValue& v, const Vec3& def)
{
    if (!v.IsArray() || v.Size() < 3) return def;
    return Vec3(
        static_cast<float32>(v[0].AsNumber(def.x)),
        static_cast<float32>(v[1].AsNumber(def.y)),
        static_cast<float32>(v[2].AsNumber(def.z)));
}

} // namespace Good