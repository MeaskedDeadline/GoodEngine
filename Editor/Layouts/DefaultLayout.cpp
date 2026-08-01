#include "Layouts/DefaultLayout.h"

namespace Good
{

    void DefaultLayout::Apply(uint32 W, uint32 H)
    {
        m_Regions.clear();

        float32 w = static_cast<float32>(W);
        float32 h = static_cast<float32>(H);

        const float32 titleBarH = 75.0f;
        const float32 menuH     = 24.0f;
        const float32 toolbarH  = 44.0f;
        const float32 hierW     = 300.0f;
        const float32 inspW     = 340.0f;
        const float32 bottomH   = 240.0f;
        const float32 gap       = 6.0f;

        AddRegion({MENUBAR, Rect(0, titleBarH, w, menuH), true, false});
        AddRegion({TOOLBAR, Rect(0, titleBarH + menuH, w, toolbarH), true, false});

        float32 contentTop = titleBarH + menuH + toolbarH + gap;
        float32 contentBottom = h - bottomH - gap * 2;
        float32 contentH = contentBottom - contentTop;

        AddRegion({HIERARCHY,
            Rect(gap, contentTop, hierW, contentH), true, true});

        float32 viewX = hierW + gap * 2;
        float32 viewW = w - hierW - inspW - gap * 4;
        AddRegion({VIEWPORT,
            Rect(viewX, contentTop, viewW, contentH), true, true});

        float32 inspX = viewX + viewW + gap;
        AddRegion({INSPECTOR,
            Rect(inspX, contentTop, inspW, contentH), true, true});

        float32 bottomTop = contentBottom + gap;
        float32 halfW = (w - gap * 3) * 0.5f;

        AddRegion({CONTENT,
            Rect(gap, bottomTop, halfW, bottomH), true, true});
        AddRegion({CONSOLE,
            Rect(gap + halfW + gap, bottomTop, halfW, bottomH), true, true});
    }

} // namespace Good