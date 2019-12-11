#include "sketch/ConsEditView.h"

#include <geoshape/Point2D.h>
#include <geoshape/Line2D.h>

#include <thread>
#include <iostream>

namespace sketch
{

ConsEditView::ConsEditView(const ee0::SceneNodeContainer& nodes,
                           const ee0::SubjectMgrPtr& sub_mgr)
    : draft2::CommonEditView(nodes, sub_mgr)
{
}

void ConsEditView::Insert(const std::shared_ptr<gs::Shape2D>& shape)
{
    draft2::CommonEditView::Insert(shape);

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
    draft2::CommonEditView::Clear();

    m_layout.Clear();
    m_shape2geo.clear();
}

void ConsEditView::AddDistanceConstraint(const std::shared_ptr<gs::Line2D>& line, float distance)
{
    auto itr = m_shape2geo.find(line);
    assert(itr != m_shape2geo.end());

    auto geo = itr->second;
    for (auto& c : geo->cons) {
        if (c->type == ConstriantsType::Distance) {
            c->val = distance;
            return;
        }
    }

    auto cons = std::make_shared<Constraint>();
    cons->type = ConstriantsType::Distance;
    cons->val = distance;
    cons->id = m_layout.AddDistanceConstraint(geo->id, &cons->val);

    m_layout.Solve();

    geo->cons.push_back(cons);
}

void ConsEditView::AddVerticalConstraint(const std::shared_ptr<gs::Line2D>& line)
{
    auto itr = m_shape2geo.find(line);
    assert(itr != m_shape2geo.end());

    auto geo = itr->second;
    for (auto& c : geo->cons) {
        if (c->type == ConstriantsType::Vertical) {
            return;
        }
    }

    auto cons = std::make_shared<Constraint>();
    cons->type = ConstriantsType::Vertical;
    cons->id = m_layout.AddVerticalConstraint(geo->id);

    m_layout.Solve();

    geo->cons.push_back(cons);
}

void ConsEditView::AddHorizontalConstraint(const std::shared_ptr<gs::Line2D>& line)
{
    auto itr = m_shape2geo.find(line);
    assert(itr != m_shape2geo.end());

    auto geo = itr->second;
    for (auto& c : geo->cons) {
        if (c->type == ConstriantsType::Horizontal) {
            return;
        }
    }

    auto cons = std::make_shared<Constraint>();
    cons->type = ConstriantsType::Horizontal;
    cons->id = m_layout.AddHorizontalConstraint(geo->id);

    m_layout.Solve();

    geo->cons.push_back(cons);
}

void ConsEditView::ClearConstraints()
{
    m_layout.ClearConstraints();

    for (auto itr : m_shape2geo) {
        itr.second->cons.clear();
    }
}

}