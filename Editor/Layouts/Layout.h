#pragma once
#include "Core/Core.h"
#include "Math/Rect.h"

namespace Good
{

    struct LayoutRegion
    {
        std::string name;
        Rect        rect{0,0,0,0};
        bool        visible{true};
        bool        resizable{true};
        float32     minWidth{100.0f};
        float32     minHeight{80.0f};
    };

    class Layout
    {
    public:
        virtual ~Layout() = default;

        virtual void Apply(uint32 viewportW, uint32 viewportH) = 0;
        virtual const char* GetName() const = 0;

        const LayoutRegion* GetRegion(const std::string& name) const;
        LayoutRegion*       GetRegion(const std::string& name);
        void                SetRegion(const LayoutRegion& region);

        const std::vector<LayoutRegion>& GetRegions() const { return m_Regions; }

    protected:
        void AddRegion(const LayoutRegion& region);
        std::vector<LayoutRegion> m_Regions;
    };

} // namespace Good