#include "sketch/RightPopupMenu.h"
#include "sketch/ConsEditView.h"

#include <ee0/EditPanelImpl.h>
#include <ee0/WxStagePage.h>

#include <SM_Calc.h>
#include <geoshape/Shape2D.h>
#include <geoshape/Point2D.h>
#include <geoshape/Line2D.h>
#include <node0/SceneNode.h>
#include <node2/CompShape.h>

#include <wx/window.h>

namespace
{

enum MenuID
{
    // Geometric constraints
    ConsHorizontal = 20000,
    ConsVertical,

    // Dimensional constraints
    ConsDistance,

    ClearAll,
};

}

namespace sketch
{

RightPopupMenu::RightPopupMenu(ee0::WxStagePage* stage, ConsEditView& view)
    : ee0::RightPopupMenu(stage, &stage->GetImpl())
    , m_stage(stage)
    , m_view(view)
{
}

void RightPopupMenu::SetRightPopupMenu(wxMenu& menu, int x, int y)
{
    std::vector<std::shared_ptr<gs::Shape2D>> shapes;
    GetAllShapes(shapes);

    if (HaveConstriants(shapes, ConstriantsType::Horizontal))
    {
        m_stage->Bind(wxEVT_COMMAND_MENU_SELECTED,
            &ee0::EditPanelImpl::OnRightPopupMenu, &m_stage->GetImpl(), ConsHorizontal);
        menu.Append(ConsHorizontal, "Horizontal", "Line length or Points horizontal");
    }
    if (HaveConstriants(shapes, ConstriantsType::Vertical))
    {
        m_stage->Bind(wxEVT_COMMAND_MENU_SELECTED,
            &ee0::EditPanelImpl::OnRightPopupMenu, &m_stage->GetImpl(), ConsVertical);
        menu.Append(ConsVertical, "Vertical", "Line length or Points vertical");
    }
    if (HaveConstriants(shapes, ConstriantsType::Distance))
    {
        m_stage->Bind(wxEVT_COMMAND_MENU_SELECTED,
            &ee0::EditPanelImpl::OnRightPopupMenu, &m_stage->GetImpl(), ConsDistance);
        menu.Append(ConsDistance, "Distance", "Line length or Points distance");
    }

    menu.AppendSeparator();

    m_stage->Bind(wxEVT_COMMAND_MENU_SELECTED,
        &ee0::EditPanelImpl::OnRightPopupMenu, &m_stage->GetImpl(), ClearAll);
    menu.Append(ClearAll, "Clear", "Clear all Constriants");
}

void RightPopupMenu::OnRightPopupMenu(int id)
{
    std::vector<std::shared_ptr<gs::Shape2D>> shapes;
    GetAllShapes(shapes);

    switch (id)
    {
    case ConsHorizontal:
    {
        if (shapes.size() == 1 && shapes[0]->get_type() == rttr::type::get<gs::Line2D>())
        {
            auto line = std::static_pointer_cast<gs::Line2D>(shapes[0]);
            m_view.AddHorizontalConstraint(line);
        }
    }
        break;
    case ConsVertical:
    {
        if (shapes.size() == 1 && shapes[0]->get_type() == rttr::type::get<gs::Line2D>())
        {
            auto line = std::static_pointer_cast<gs::Line2D>(shapes[0]);
            m_view.AddVerticalConstraint(line);
        }
    }
        break;
    case ConsDistance:
    {
        if (shapes.size() == 1 && shapes[0]->get_type() == rttr::type::get<gs::Line2D>())
        {
            auto line = std::static_pointer_cast<gs::Line2D>(shapes[0]);
            auto dist = sm::dis_pos_to_pos(line->GetStart(), line->GetEnd());
            m_view.AddDistanceConstraint(line, dist);
        }
    }
        break;
    case ClearAll:
        m_view.ClearConstraints();
        break;
    }
}

void RightPopupMenu::GetAllShapes(std::vector<std::shared_ptr<gs::Shape2D>>& shapes) const
{
    m_stage->GetSelection().Traverse([&](const ee0::GameObjWithPos& nwp)->bool
    {
        auto node = nwp.GetNode();
        if (!node->HasSharedComp<n2::CompShape>()) {
            return true;
        }

        auto& cshape = node->GetSharedComp<n2::CompShape>();
        auto& shape = cshape.GetShape();
        if (shape) {
            shapes.push_back(shape);
        }

        return true;
    });
}

bool RightPopupMenu::HaveConstriants(const std::vector<std::shared_ptr<gs::Shape2D>>& shapes, ConstriantsType type)
{
    switch (type)
    {
    case ConstriantsType::Horizontal:
    case ConstriantsType::Vertical:
    case ConstriantsType::Distance:
        return (shapes.size() == 1 && shapes[0]->get_type() == rttr::type::get<gs::Line2D>())
            || (shapes.size() == 2 && shapes[0]->get_type() == rttr::type::get<gs::Point2D>()
                && shapes[1]->get_type() == rttr::type::get<gs::Point2D>());
    }

    return false;
}

}