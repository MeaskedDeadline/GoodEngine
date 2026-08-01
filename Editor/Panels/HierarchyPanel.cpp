#include "Panels/HierarchyPanel.h"
#include "UI/UIContext.h"
#include "UI/UIRenderer.h"
#include "UI/UILayout.h"
#include "UI/UIInput.h"
#include "UI/Widgets/UILabel.h"
#include "UI/Widgets/UIButton.h"
#include "UI/Widgets/UITextInput.h"
#include "UI/Widgets/UITreeNode.h"
#include "Input/Input.h"
#include "Input/KeyCodes.h"
#include "Input/MouseCodes.h"
#include "Core/UndoSystem.h"
#include "Math/MathCommon.h"

namespace Good
{

HierarchyPanel::HierarchyPanel() = default;

void HierarchyPanel::OnUpdate(float32)
{
    if (m_Selected && Input::Get().IsKeyPressed(Key::Delete))
        DeleteSelected();

    if (m_Selected && Input::Get().IsKeyDown(Key::LeftCtrl) &&
        Input::Get().IsKeyPressed(Key::D))
        DuplicateSelected();
}

void HierarchyPanel::CreateObject(NodeType type, const std::string& name)
{
    if (!m_Scene) return;

    Ref<SceneNode> parent = m_Selected ? m_Selected : m_Scene->GetRoot();
    auto node = m_Scene->CreateNode(name, parent);
    node->SetType(type);

    switch (type)
    {
        case NodeType::Cube:
            node->SetColor(Color(0.8f, 0.3f, 0.3f, 1.0f));
            node->GetLocalTransform().position = Vec3(0, 0.5f, 0);
            break;
        case NodeType::Sphere:
            node->SetColor(Color(0.3f, 0.6f, 0.9f, 1.0f));
            node->GetLocalTransform().position = Vec3(0, 0.5f, 0);
            break;
        case NodeType::Plane:
            node->SetColor(Color(0.5f, 0.5f, 0.5f, 1.0f));
            node->GetLocalTransform().scale = Vec3(5, 1, 5);
            break;
        case NodeType::Capsule:
            node->SetColor(Color(0.3f, 0.8f, 0.4f, 1.0f));
            node->GetLocalTransform().position = Vec3(0, 1.0f, 0);
            break;
        case NodeType::DirectionalLight:
            node->GetLocalTransform().rotation = Quat::FromEuler(-45, 45, 0);
            break;
        case NodeType::PointLight:
            node->GetLocalTransform().position = Vec3(0, 2, 0);
            break;
        case NodeType::Camera:
            node->GetLocalTransform().position = Vec3(0, 2, 8);
            break;
        default: break;
    }

    m_Selected = node;
    m_ShowContextMenu = false;
}

void HierarchyPanel::DeleteSelected()
{
    if (!m_Selected || !m_Scene) return;

    auto node   = m_Selected;
    auto parent = node->GetParent();
    if (!parent) return;

    Ref<SceneNode> parentRef;
    m_Scene->GetGraph().Traverse([&](Ref<SceneNode> n)
    {
        if (n.get() == parent) parentRef = n;
    });

    if (!parentRef) return;

    UndoSystem::Get().Execute(
        "Delete " + node->GetName(),
        [parentRef, node]() { parentRef->RemoveChild(node); },
        [parentRef, node]() { parentRef->AddChild(node); }
    );

    m_Selected = nullptr;
}

void HierarchyPanel::DuplicateSelected()
{
    if (!m_Selected || !m_Scene) return;

    auto parent = m_Selected->GetParent();
    Ref<SceneNode> parentRef;

    if (parent)
    {
        m_Scene->GetGraph().Traverse([&](Ref<SceneNode> n)
        {
            if (n.get() == parent) parentRef = n;
        });
    }

    if (!parentRef) parentRef = m_Scene->GetRoot();

    auto copy = m_Scene->CreateNode(m_Selected->GetName() + " Copy", parentRef);
    copy->SetType(m_Selected->GetType());
    copy->SetColor(m_Selected->GetColor());
    copy->GetLocalTransform() = m_Selected->GetLocalTransform();
    copy->GetLocalTransform().position += Vec3(1, 0, 0);

    m_Selected = copy;
}

void HierarchyPanel::DrawContextMenu()
{
    if (!m_ShowContextMenu) return;

    auto& ctx      = UIContext::Get();
    auto& renderer = UIRenderer::Get();
    auto& style    = ctx.GetStyle();

    float32 menuW = 200.0f;
    float32 itemH = 28.0f;
    int32 itemCount = m_ContextOnNode ? 3 : 8;
    float32 menuH = static_cast<float32>(itemCount) * itemH + 8.0f;

    Rect menuRect(m_ContextMenuX, m_ContextMenuY, menuW, menuH);

    renderer.DrawRect(menuRect, Color(0.15f, 0.15f, 0.18f, 0.98f));
    renderer.DrawRectOutline(menuRect, style.colors.border, 1.0f);

    float32 y = menuRect.y + 4.0f;

    auto drawMenuItem = [&](const char* label) -> bool
    {
        Rect itemRect(menuRect.x + 2, y, menuW - 4, itemH);
        bool hovered = ctx.IsItemHovered(itemRect);

        if (hovered)
            renderer.DrawRect(itemRect, style.colors.selection, 2.0f);

        renderer.DrawTextUI(label,
            Vec2(menuRect.x + 14, y + 6),
            hovered ? style.colors.textSelected : style.colors.text,
            style.metrics.fontSize);

        y += itemH;
        return hovered && ctx.GetState().mousePressed;
    };

    if (m_ContextOnNode)
    {
        if (drawMenuItem("Delete"))    { DeleteSelected();    m_ShowContextMenu = false; }
        if (drawMenuItem("Duplicate")) { DuplicateSelected(); m_ShowContextMenu = false; }
        if (drawMenuItem("Add Child")) { CreateObject(NodeType::Empty, "Empty"); }
    }
    else
    {
        if (drawMenuItem("Empty"))             CreateObject(NodeType::Empty,            "Empty");
        if (drawMenuItem("Cube"))              CreateObject(NodeType::Cube,             "Cube");
        if (drawMenuItem("Sphere"))            CreateObject(NodeType::Sphere,           "Sphere");
        if (drawMenuItem("Plane"))             CreateObject(NodeType::Plane,            "Plane");
        if (drawMenuItem("Capsule"))           CreateObject(NodeType::Capsule,          "Capsule");
        if (drawMenuItem("Directional Light")) CreateObject(NodeType::DirectionalLight, "Dir Light");
        if (drawMenuItem("Point Light"))       CreateObject(NodeType::PointLight,       "Point Light");
        if (drawMenuItem("Camera"))            CreateObject(NodeType::Camera,           "Camera");
    }

    if (ctx.GetState().mousePressed && !ctx.IsItemHovered(menuRect))
        m_ShowContextMenu = false;
}

void HierarchyPanel::DrawToolbar()
{
    auto& renderer = UIRenderer::Get();
    auto& style    = UIContext::Get().GetStyle();

    Rect headerRect(m_Rect.x, m_Rect.y, m_Rect.width, 28.0f);
    renderer.DrawRect(headerRect, style.colors.titleBar);
    renderer.DrawTextUI("HIERARCHY",
        Vec2(m_Rect.x + 12, m_Rect.y + 7),
        style.colors.text, 14.0f);

    Rect searchBar(m_Rect.x, m_Rect.y + 28, m_Rect.width, 34.0f);
    renderer.DrawRect(searchBar, style.colors.header);

    UILayout::Get().PushLayout(Rect(m_Rect.x + 8, m_Rect.y + 34,
                                    m_Rect.width - 50, 22));
    UITextInput::Draw("", m_SearchText, "Search...", m_Rect.width - 50);
    UILayout::Get().PopLayout();

    UILayout::Get().PushLayout(Rect(m_Rect.x + m_Rect.width - 36,
                                    m_Rect.y + 34, 28, 22));
    if (UIButton::Draw("+", Vec2(28, 22)))
    {
        m_ShowContextMenu  = true;
        m_ContextOnNode    = false;
        m_ContextMenuX     = m_Rect.x + m_Rect.width - 200;
        m_ContextMenuY     = m_Rect.y + 60;
    }
    UILayout::Get().PopLayout();
}

void HierarchyPanel::OnRender()
{
    if (!m_Visible) return;

    auto& ctx      = UIContext::Get();
    auto& renderer = UIRenderer::Get();
    auto& style    = ctx.GetStyle();

    // Рисуем рамку панели с title bar
    DrawPanelFrame();

    // Toolbar под title bar
    Rect searchBar(m_Rect.x, m_Rect.y + TITLE_BAR_H, m_Rect.width, 34.0f);
    renderer.DrawRect(searchBar, style.colors.header);

    UILayout::Get().PushLayout(Rect(m_Rect.x + 8, m_Rect.y + TITLE_BAR_H + 6,
                                    m_Rect.width - 50, 22));
    UITextInput::Draw("", m_SearchText, "Search...", m_Rect.width - 50);
    UILayout::Get().PopLayout();

    UILayout::Get().PushLayout(Rect(m_Rect.x + m_Rect.width - 36,
                                    m_Rect.y + TITLE_BAR_H + 6, 28, 22));
    if (UIButton::Draw("+", Vec2(28, 22)))
    {
        m_ShowContextMenu  = true;
        m_ContextOnNode    = false;
        m_ContextMenuX     = m_Rect.x + m_Rect.width - 200;
        m_ContextMenuY     = m_Rect.y + TITLE_BAR_H + 34;
    }
    UILayout::Get().PopLayout();

    // Контент
    float32 contentY = m_Rect.y + TITLE_BAR_H + 38;
    float32 contentH = m_Rect.height - TITLE_BAR_H - 44;
    Rect contentRect(m_Rect.x + 6, contentY,
                     m_Rect.width - 12, contentH);

    // Скролл
    if (ctx.IsItemHovered(contentRect))
    {
        float32 scroll = UIInput::Get().GetScrollY();
        if (Math::Abs(scroll) > 0.01f)
        {
            m_ScrollY -= scroll * 30.0f;
            float32 maxScroll = Math::Max(0.0f,
                static_cast<float32>(m_NodeCount) * 24.0f - contentH);
            m_ScrollY = Math::Clamp(m_ScrollY, 0.0f, maxScroll);
        }
    }

    // ПКМ
    if (ctx.IsItemHovered(contentRect) &&
        Input::Get().IsMouseButtonPressed(MouseButton::Right))
    {
        m_ShowContextMenu = true;
        m_ContextOnNode   = false;
        m_ContextMenuX    = Input::Get().GetMouseX();
        m_ContextMenuY    = Input::Get().GetMouseY();
    }

    UILayout::Get().PushLayout(Rect(contentRect.x, contentRect.y - m_ScrollY,
                                    contentRect.width, 10000.0f));
    m_NodeCount = 0;

    if (m_Scene)
    {
        auto root = m_Scene->GetRoot();
        for (auto& child : root->GetChildren())
            DrawNode(child, 0);
    }
    else
    {
        UILabel::DrawColored("No scene loaded", style.colors.textDisabled);
    }

    UILayout::Get().PopLayout();

    DrawContextMenu();
}

void HierarchyPanel::DrawNode(const Ref<SceneNode>& node, int32 depth)
{
    if (!node) return;

    if (!m_SearchText.empty())
    {
        if (node->GetName().find(m_SearchText) == std::string::npos &&
            node->GetChildren().empty())
            return;
    }

    m_NodeCount++;

    bool isSelected  = (node == m_Selected);
    bool hasChildren = !node->GetChildren().empty();

    const char* typeIcon = "";
    switch (node->GetType())
    {
        case NodeType::Cube:             typeIcon = "[C] "; break;
        case NodeType::Sphere:           typeIcon = "[S] "; break;
        case NodeType::Plane:            typeIcon = "[P] "; break;
        case NodeType::Capsule:          typeIcon = "[K] "; break;
        case NodeType::DirectionalLight: typeIcon = "[L] "; break;
        case NodeType::PointLight:       typeIcon = "[o] "; break;
        case NodeType::SpotLight:        typeIcon = "[v] "; break;
        case NodeType::Camera:           typeIcon = "[M] "; break;
        default:                         typeIcon = "[ ] "; break;
    }

    std::string label = typeIcon + node->GetName();
    if (!node->IsActive()) label = "[H] " + label;

    Vec2 cursor = UILayout::Get().GetCursor();
    Rect nodeRect(cursor.x, cursor.y, m_Rect.width - 20, 22);

    // ПКМ на узле
    if (UIContext::Get().IsItemHovered(nodeRect) &&
        Input::Get().IsMouseButtonPressed(MouseButton::Right))
    {
        m_Selected        = node;
        m_ShowContextMenu = true;
        m_ContextOnNode   = true;
        m_ContextMenuX    = Input::Get().GetMouseX();
        m_ContextMenuY    = Input::Get().GetMouseY();
    }

    if (hasChildren)
    {
        if (UITreeNode::Begin(label, true, isSelected ? &isSelected : nullptr))
        {
            if (isSelected) m_Selected = node;
            for (auto& child : node->GetChildren())
                DrawNode(child, depth + 1);
            UITreeNode::End();
        }
    }
    else
    {
        if (UITreeNode::BeginLeaf(label, isSelected ? &isSelected : nullptr))
            m_Selected = node;
    }
}

} // namespace Good