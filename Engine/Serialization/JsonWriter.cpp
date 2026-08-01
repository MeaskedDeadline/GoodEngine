#include "Serialization/JsonWriter.h"
#include <fstream>

namespace Good
{

std::string JsonWriter::Write(const JsonValue& value, bool pretty, int32 indentSize)
{
    std::string out;
    WriteValue(value, out, 0, pretty, indentSize);
    return out;
}

bool JsonWriter::WriteToFile(const JsonValue& value, const std::string& path,
                              bool pretty, int32 indentSize)
{
    std::ofstream file(path);
    if (!file.is_open()) return false;
    file << Write(value, pretty, indentSize);
    return true;
}

void JsonWriter::WriteValue(const JsonValue& value, std::string& out,
                             int32 depth, bool pretty, int32 indentSize)
{
    switch (value.GetType())
    {
        case JsonValue::Type::Null:
            out += "null";
            break;

        case JsonValue::Type::Bool:
            out += value.AsBool() ? "true" : "false";
            break;

        case JsonValue::Type::Number:
            out += std::to_string(value.AsNumber());
            break;

        case JsonValue::Type::String:
            out += "\"";
            out += EscapeString(value.AsString());
            out += "\"";
            break;

        case JsonValue::Type::Array:
        {
            const auto& arr = value.AsArray();
            out += "[";
            if (!arr.empty())
            {
                if (pretty) out += "\n";
                for (usize i = 0; i < arr.size(); ++i)
                {
                    if (pretty) WriteIndent(out, depth + 1, indentSize);
                    WriteValue(arr[i], out, depth + 1, pretty, indentSize);
                    if (i + 1 < arr.size()) out += ",";
                    if (pretty) out += "\n";
                }
                if (pretty) WriteIndent(out, depth, indentSize);
            }
            out += "]";
            break;
        }

        case JsonValue::Type::Object:
        {
            const auto& obj = value.AsObject();
            out += "{";
            if (!obj.empty())
            {
                if (pretty) out += "\n";
                usize count = 0;
                for (const auto& [key, val] : obj)
                {
                    if (pretty) WriteIndent(out, depth + 1, indentSize);
                    out += "\"";
                    out += EscapeString(key);
                    out += "\":";
                    if (pretty) out += " ";
                    WriteValue(val, out, depth + 1, pretty, indentSize);
                    ++count;
                    if (count < obj.size()) out += ",";
                    if (pretty) out += "\n";
                }
                if (pretty) WriteIndent(out, depth, indentSize);
            }
            out += "}";
            break;
        }
    }
}

void JsonWriter::WriteIndent(std::string& out, int32 depth, int32 indentSize)
{
    for (int32 i = 0; i < depth * indentSize; ++i)
        out.push_back(' ');
}

std::string JsonWriter::EscapeString(const std::string& value)
{
    std::string out;
    out.reserve(value.size());
    for (char c : value)
    {
        switch (c)
        {
            case '\"': out += "\\\""; break;
            case '\\': out += "\\\\"; break;
            case '\b': out += "\\b";  break;
            case '\f': out += "\\f";  break;
            case '\n': out += "\\n";  break;
            case '\r': out += "\\r";  break;
            case '\t': out += "\\t";  break;
            default:   out += c;      break;
        }
    }
    return out;
}

} // namespace Good