#pragma once
#include "Serialization/JsonValue.h"

namespace Good
{

    class JsonWriter
    {
    public:
        static std::string Write(const JsonValue& value,
                                  bool pretty = true,
                                  int32 indentSize = 4);

        static bool WriteToFile(const JsonValue& value,
                                 const std::string& path,
                                 bool pretty = true,
                                 int32 indentSize = 4);

    private:
        static void WriteValue(const JsonValue& value, std::string& out,
                               int32 depth, bool pretty, int32 indentSize);

        static void        WriteIndent(std::string& out, int32 depth, int32 indentSize);
        static std::string EscapeString(const std::string& value);
    };

} // namespace Good