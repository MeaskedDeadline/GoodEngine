#pragma once
#include "Serialization/JsonValue.h"

namespace Good
{

    class JsonParser
    {
    public:
        static bool Parse(const std::string& text, JsonValue& outValue,
                          std::string* outError = nullptr);

        static bool ParseFile(const std::string& path, JsonValue& outValue,
                              std::string* outError = nullptr);

    private:
        JsonParser(const std::string& text);

        bool ParseRoot(JsonValue& outValue);
        bool ParseValue(JsonValue& outValue);
        bool ParseObject(JsonValue& outValue);
        bool ParseArray(JsonValue& outValue);
        bool ParseString(std::string& outString);
        bool ParseNumber(JsonValue& outValue);
        bool ParseTrue(JsonValue& outValue);
        bool ParseFalse(JsonValue& outValue);
        bool ParseNull(JsonValue& outValue);

        void SkipWhitespace();
        bool Match(char c);
        char Peek() const;
        char Get();
        bool End() const;
        void SetError(const std::string& error);

        const std::string& m_Text;
        usize              m_Index{0};
        std::string        m_Error;
    };

} // namespace Good