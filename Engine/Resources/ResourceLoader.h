#pragma once
#include "../Core/Core.h"

namespace Good
{

    class ResourceLoader
    {
    public:
        virtual ~ResourceLoader() = default;

        virtual Ref<void> Load(const std::string& path)     = 0;
        virtual bool      CanLoad(const std::string& ext)   const = 0;
        virtual const char* GetName() const { return "ResourceLoader"; }
    };

} // namespace Good