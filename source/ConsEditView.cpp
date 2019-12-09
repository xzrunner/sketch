#include "sketch/ConsEditView.h"

#include <geoshape/Point2D.h>
#include <geoshape/Line2D.h>

namespace sketch
{

ConsEditView::ConsEditView(const ee0::SceneNodeContainer& nodes,
                           const ee0::SubjectMgrPtr& sub_mgr)
    : dw2::CommonEditView(nodes, sub_mgr)
{
}

void ConsEditView::Insert(const std::shared_ptr<gs::Shape2D>& shape)
{
    dw2::CommonEditView::Insert(shape);

    auto type = shape->get_type();
    size_t id = 0;
    if (type == rttr::type::get<gs::Point2D>()) {
        id = m_layout.AddPoint(std::static_pointer_cast<gs::Point2D>(shape));
    } else if (type == rttr::type::get<gs::Line2D>()) {
        id = m_layout.AddLine(std::static_pointer_cast<gs::Line2D>(shape));
    } else {
        return;
    }

    auto geo = std::make_shared<Geometry>();
    geo->id = id;
    geo->shape = shape;
    m_shape2geo.insert({ shape, geo });
}

void ConsEditView::Clear()
{
    dw2::CommonEditView::Clear();

    m_layout.Clear();
    m_shape2geo.clear();
}

void ConsEditView::ShapeChanged(const std::shared_ptr<gs::Shape2D>& shape)
{
    dw2::CommonEditView::ShapeChanged(shape);

    m_layout.Solve();
}

void ConsEditView::AddDistanceConstraint(const std::shared_ptr<gs::Line2D>& line, float distance)
{
    auto itr = m_shape2geo.find(line);
    assert(itr != m_shape2geo.end());

    auto geo = itr->second;
    for (auto& c : geo->cons) {
        if (c.type == ConstriantsType::Distance) {
            c.val = distance;
            return;
        }
    }

    Constraint cons;
    cons.type = ConstriantsType::Distance;
    cons.val = distance;
    cons.id = m_layout.AddDistanceConstraint(geo->id, &cons.val);

    geo->cons.push_back(cons);
}

}