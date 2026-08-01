#include "Serialization/JsonParser.h"
#include <fstream>
#include <sstream>
#include <cstdlib>

namespace Good
{

JsonParser::JsonParser(const std::string& text) : m_Text(text) {}

bool JsonParser::Parse(const std::string& text, JsonValue& out, std::string* err)
{
    JsonParser p(text);
    bool ok = p.ParseRoot(out);
    if (!ok && err) *err = p.m_Error;
    return ok;
}

bool JsonParser::ParseFile(const std::string& path, JsonValue& out, std::string* err)
{
    std::ifstream file(path);
    if (!file.is_open())
    {
        if (err) *err = "Cannot open: " + path;
        return false;
    }
    std::stringstream ss;
    ss << file.rdbuf();
    return Parse(ss.str(), out, err);
}

bool JsonParser::ParseRoot(JsonValue& out)
{
    SkipWhitespace();
    if (!ParseValue(out)) return false;
    SkipWhitespace();
    if (!End()) { SetError("Unexpected trailing characters"); return false; }
    return true;
}

bool JsonParser::ParseValue(JsonValue& out)
{
    SkipWhitespace();
    char c = Peek();

    if (c == '{') return ParseObject(out);
    if (c == '[') return ParseArray(out);
    if (c == '"')
    {
        std::string s;
        if (!ParseString(s)) return false;
        out = JsonValue(std::move(s));
        return true;
    }
    if (c == 't') return ParseTrue(out);
    if (c == 'f') return ParseFalse(out);
    if (c == 'n') return ParseNull(out);
    if (c == '-' || (c >= '0' && c <= '9')) return ParseNumber(out);

    SetError("Unexpected token");
    return false;
}

bool JsonParser::ParseObject(JsonValue& out)
{
    if (!Match('{')) { SetError("Expected '{'"); return false; }
    out = JsonValue::MakeObject();
    SkipWhitespace();
    if (Match('}')) return true;

    while (true)
    {
        SkipWhitespace();
        std::string key;
        if (!ParseString(key)) return false;
        SkipWhitespace();
        if (!Match(':')) { SetError("Expected ':'"); return false; }

        JsonValue val;
        if (!ParseValue(val)) return false;
        out[key] = std::move(val);

        SkipWhitespace();
        if (Match('}')) break;
        if (!Match(',')) { SetError("Expected ',' or '}'"); return false; }
    }
    return true;
}

bool JsonParser::ParseArray(JsonValue& out)
{
    if (!Match('[')) { SetError("Expected '['"); return false; }
    out = JsonValue::MakeArray();
    SkipWhitespace();
    if (Match(']')) return true;

    while (true)
    {
        JsonValue val;
        if (!ParseValue(val)) return false;
        out.PushBack(std::move(val));
        SkipWhitespace();
        if (Match(']')) break;
        if (!Match(',')) { SetError("Expected ',' or ']'"); return false; }
    }
    return true;
}

bool JsonParser::ParseString(std::string& out)
{
    if (!Match('"')) { SetError("Expected string"); return false; }
    out.clear();

    while (!End())
    {
        char c = Get();
        if (c == '"') return true;
        if (c == '\\')
        {
            if (End()) { SetError("Unexpected end"); return false; }
            char e = Get();
            switch (e)
            {
                case '"':  out += '"';  break;
                case '\\': out += '\\'; break;
                case '/':  out += '/';  break;
                case 'b':  out += '\b'; break;
                case 'f':  out += '\f'; break;
                case 'n':  out += '\n'; break;
                case 'r':  out += '\r'; break;
                case 't':  out += '\t'; break;
                case 'u':
                    for (int i = 0; i < 4; ++i) { if (End()) return false; Get(); }
                    out += '?';
                    break;
                default: SetError("Unknown escape"); return false;
            }
        }
        else
        {
            out += c;
        }
    }
    SetError("Unterminated string");
    return false;
}

bool JsonParser::ParseNumber(JsonValue& out)
{
    usize start = m_Index;
    if (Peek() == '-') ++m_Index;
    while (!End() && std::isdigit(static_cast<unsigned char>(Peek()))) ++m_Index;
    if (!End() && Peek() == '.')
    {
        ++m_Index;
        while (!End() && std::isdigit(static_cast<unsigned char>(Peek()))) ++m_Index;
    }
    if (!End() && (Peek() == 'e' || Peek() == 'E'))
    {
        ++m_Index;
        if (!End() && (Peek() == '+' || Peek() == '-')) ++m_Index;
        while (!End() && std::isdigit(static_cast<unsigned char>(Peek()))) ++m_Index;
    }

    std::string num = m_Text.substr(start, m_Index - start);
    char* ep = nullptr;
    double v = std::strtod(num.c_str(), &ep);
    if (ep == num.c_str()) { SetError("Invalid number"); return false; }
    out = JsonValue(static_cast<float64>(v));
    return true;
}

bool JsonParser::ParseTrue(JsonValue& out)
{
    if (m_Text.substr(m_Index, 4) == "true") { m_Index += 4; out = JsonValue(true); return true; }
    SetError("Expected 'true'"); return false;
}

bool JsonParser::ParseFalse(JsonValue& out)
{
    if (m_Text.substr(m_Index, 5) == "false") { m_Index += 5; out = JsonValue(false); return true; }
    SetError("Expected 'false'"); return false;
}

bool JsonParser::ParseNull(JsonValue& out)
{
    if (m_Text.substr(m_Index, 4) == "null") { m_Index += 4; out = JsonValue(nullptr); return true; }
    SetError("Expected 'null'"); return false;
}

void JsonParser::SkipWhitespace()
{
    while (!End() && std::isspace(static_cast<unsigned char>(Peek()))) ++m_Index;
}

bool JsonParser::Match(char c)
{
    if (!End() && Peek() == c) { ++m_Index; return true; }
    return false;
}

char JsonParser::Peek() const { return End() ? '\0' : m_Text[m_Index]; }
char JsonParser::Get()        { return End() ? '\0' : m_Text[m_Index++]; }
bool JsonParser::End()  const { return m_Index >= m_Text.size(); }

void JsonParser::SetError(const std::string& e)
{
    m_Error = e + " at index " + std::to_string(m_Index);
}

} // namespace Good