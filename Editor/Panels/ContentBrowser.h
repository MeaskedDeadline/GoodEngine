#pragma once
#include "Panel.h"
#include "FileSystem/Path.h"

namespace Good
{

    class ContentBrowser final : public Panel
    {
    public:
        ContentBrowser();

        void OnRender() override;
        void OnUpdate(float32 dt) override;
        const char* GetTitle() const override { return "Content Browser"; }

        void SetRootPath(const Path& path);

    private:
        void DrawToolbar();
        void DrawBreadcrumb();
        void DrawItems();
        void DrawItem(const Path& path, bool isDir);
        void NavigateTo(const Path& path);
        void NavigateBack();

        Path              m_RootPath{"Assets"};
        Path              m_CurrentPath{"Assets"};
        std::vector<Path> m_History;
        std::vector<Path> m_Items;
        std::string       m_SearchFilter;
        float32           m_ItemSize{64.0f};
        Path              m_SelectedItem;
        bool              m_NeedsRefresh{true};
    };

} // namespace Good