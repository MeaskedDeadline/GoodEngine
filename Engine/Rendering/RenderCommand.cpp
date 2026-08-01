#include "RenderCommand.h"
#include <algorithm>

namespace Good
{

    void RenderQueue::Push(RenderCommand cmd) { m_Commands.push_back(std::move(cmd)); }
    void RenderQueue::Sort()
    {
        std::sort(m_Commands.begin(), m_Commands.end(),
            [](const RenderCommand& a, const RenderCommand& b) { return a.sortKey < b.sortKey; });
    }
    void RenderQueue::Clear() { m_Commands.clear(); }

} // namespace Good