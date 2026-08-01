#pragma once
#include "Core/Core.h"
#include <variant>

namespace Good
{

class JsonValue
{
public:
    enum class Type : uint8
    {
        Null = 0, Bool, Number, String, Array, Object
    };

    using Array  = std::vector<JsonValue>;
    using Object = std::unordered_map<std::string, JsonValue>;

    JsonValue();
    JsonValue(std::nullptr_t);
    JsonValue(bool value);
    JsonValue(int32 value);
    JsonValue(uint32 value);
    JsonValue(int64 value);
    JsonValue(uint64 value);
    JsonValue(float32 value);
    JsonValue(float64 value);
    JsonValue(const char* value);
    JsonValue(const std::string& value);
    JsonValue(std::string&& value);
    JsonValue(const Array& value);
    JsonValue(Array&& value);
    JsonValue(const Object& value);
    JsonValue(Object&& value);

    Type GetType() const;

    bool IsNull()   const;
    bool IsBool()   const;
    bool IsNumber() const;
    bool IsString() const;
    bool IsArray()  const;
    bool IsObject() const;

    bool               AsBool(bool def = false)       const;
    float64            AsNumber(float64 def = 0.0)    const;
    const std::string& AsString()                      const;
    const Array&       AsArray()                       const;
    const Object&      AsObject()                      const;

    std::string& GetString();
    Array&       GetArray();
    Object&      GetObject();

    JsonValue&       operator[](const std::string& key);
    const JsonValue& operator[](const std::string& key) const;
    JsonValue&       operator[](usize index);
    const JsonValue& operator[](usize index)             const;

    void  PushBack(const JsonValue& value);
    void  PushBack(JsonValue&& value);
    bool  Contains(const std::string& key) const;
    usize Size() const;

    static JsonValue     MakeArray();
    static JsonValue     MakeObject();
    static const JsonValue& Null();

private:
    std::variant<std::nullptr_t, bool, float64,
                 std::string, Array, Object> m_Data;
};

} // namespace Good