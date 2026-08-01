#include "Serialization/JsonValue.h"

namespace Good
{

JsonValue::JsonValue()                         : m_Data(nullptr) {}
JsonValue::JsonValue(std::nullptr_t)           : m_Data(nullptr) {}
JsonValue::JsonValue(bool v)                   : m_Data(v) {}
JsonValue::JsonValue(int32 v)                  : m_Data(static_cast<float64>(v)) {}
JsonValue::JsonValue(uint32 v)                 : m_Data(static_cast<float64>(v)) {}
JsonValue::JsonValue(int64 v)                  : m_Data(static_cast<float64>(v)) {}
JsonValue::JsonValue(uint64 v)                 : m_Data(static_cast<float64>(v)) {}
JsonValue::JsonValue(float32 v)                : m_Data(static_cast<float64>(v)) {}
JsonValue::JsonValue(float64 v)                : m_Data(v) {}
JsonValue::JsonValue(const char* v)            : m_Data(std::string(v ? v : "")) {}
JsonValue::JsonValue(const std::string& v)     : m_Data(v) {}
JsonValue::JsonValue(std::string&& v)          : m_Data(std::move(v)) {}
JsonValue::JsonValue(const Array& v)           : m_Data(v) {}
JsonValue::JsonValue(Array&& v)                : m_Data(std::move(v)) {}
JsonValue::JsonValue(const Object& v)          : m_Data(v) {}
JsonValue::JsonValue(Object&& v)               : m_Data(std::move(v)) {}

JsonValue::Type JsonValue::GetType() const
{
    if (std::holds_alternative<std::nullptr_t>(m_Data)) return Type::Null;
    if (std::holds_alternative<bool>(m_Data))           return Type::Bool;
    if (std::holds_alternative<float64>(m_Data))        return Type::Number;
    if (std::holds_alternative<std::string>(m_Data))    return Type::String;
    if (std::holds_alternative<Array>(m_Data))          return Type::Array;
    return Type::Object;
}

bool JsonValue::IsNull()   const { return GetType() == Type::Null;   }
bool JsonValue::IsBool()   const { return GetType() == Type::Bool;   }
bool JsonValue::IsNumber() const { return GetType() == Type::Number; }
bool JsonValue::IsString() const { return GetType() == Type::String; }
bool JsonValue::IsArray()  const { return GetType() == Type::Array;  }
bool JsonValue::IsObject() const { return GetType() == Type::Object; }

bool JsonValue::AsBool(bool def) const
{
    if (IsBool()) return std::get<bool>(m_Data);
    return def;
}

float64 JsonValue::AsNumber(float64 def) const
{
    if (IsNumber()) return std::get<float64>(m_Data);
    return def;
}

const std::string& JsonValue::AsString() const
{
    static const std::string s_Empty;
    if (IsString()) return std::get<std::string>(m_Data);
    return s_Empty;
}

const JsonValue::Array& JsonValue::AsArray() const
{
    static const Array s_Empty;
    if (IsArray()) return std::get<Array>(m_Data);
    return s_Empty;
}

const JsonValue::Object& JsonValue::AsObject() const
{
    static const Object s_Empty;
    if (IsObject()) return std::get<Object>(m_Data);
    return s_Empty;
}

std::string& JsonValue::GetString()
{
    if (!IsString()) m_Data = std::string();
    return std::get<std::string>(m_Data);
}

JsonValue::Array& JsonValue::GetArray()
{
    if (!IsArray()) m_Data = Array();
    return std::get<Array>(m_Data);
}

JsonValue::Object& JsonValue::GetObject()
{
    if (!IsObject()) m_Data = Object();
    return std::get<Object>(m_Data);
}

JsonValue& JsonValue::operator[](const std::string& key)
{
    return GetObject()[key];
}

const JsonValue& JsonValue::operator[](const std::string& key) const
{
    if (!IsObject()) return Null();
    auto it = std::get<Object>(m_Data).find(key);
    if (it == std::get<Object>(m_Data).end()) return Null();
    return it->second;
}

JsonValue& JsonValue::operator[](usize index)
{
    Array& arr = GetArray();
    if (index >= arr.size()) arr.resize(index + 1);
    return arr[index];
}

const JsonValue& JsonValue::operator[](usize index) const
{
    if (!IsArray()) return Null();
    const Array& arr = std::get<Array>(m_Data);
    if (index >= arr.size()) return Null();
    return arr[index];
}

void JsonValue::PushBack(const JsonValue& v) { GetArray().push_back(v); }
void JsonValue::PushBack(JsonValue&& v)      { GetArray().push_back(std::move(v)); }

bool JsonValue::Contains(const std::string& key) const
{
    if (!IsObject()) return false;
    return std::get<Object>(m_Data).find(key) != std::get<Object>(m_Data).end();
}

usize JsonValue::Size() const
{
    if (IsArray())  return std::get<Array>(m_Data).size();
    if (IsObject()) return std::get<Object>(m_Data).size();
    if (IsString()) return std::get<std::string>(m_Data).size();
    return 0;
}

JsonValue JsonValue::MakeArray()  { return Array();  }
JsonValue JsonValue::MakeObject() { return Object(); }

const JsonValue& JsonValue::Null()
{
    static JsonValue s_Null;
    return s_Null;
}

} // namespace Good