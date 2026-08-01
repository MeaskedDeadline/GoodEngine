#pragma once
#include "../Core/Core.h"

namespace Good
{

    // Биндинг C++ функций к скриптовой системе
    // Позже: Lua, Python или собственный scripting runtime

    class ScriptBind
    {
    public:
        using NativeFunction = std::function<void(const std::vector<std::string>&)>;

        static ScriptBind& Get();

        void RegisterFunction(const std::string& name, NativeFunction fn);
        bool CallFunction(const std::string& name, const std::vector<std::string>& args);
        bool HasFunction(const std::string& name) const;

        const std::unordered_map<std::string, NativeFunction>& GetAll() const { return m_Functions; }

    private:
        ScriptBind() = default;

        std::unordered_map<std::string, NativeFunction> m_Functions;
    };

} // namespace Good