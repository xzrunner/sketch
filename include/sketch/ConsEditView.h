#pragma once

#include "sketch/ConstriantsType.h"

#include <drawing2/CommonEditView.h>
#include <constraints2/Scene.h>

namespace sketch
{

class ConsEditView : public dw2::CommonEditView
{
public:
    ConsEditView(const ee0::SceneNodeContainer& nodes,
        const ee0::SubjectMgrPtr& sub_mgr);

    virtual void Insert(const std::shared_ptr<gs::Shape2D>& shape) override;
    virtual void Clear() override;

    virtual void ShapeChanged(const std::shared_ptr<gs::Shape2D>& shape) override;

    void AddDistanceConstraint(const std::shared_ptr<gs::Line2D>& line, float distance);

private:
    struct Constraint
    {
        size_t id = 0;
        ConstriantsType type;

        double val = 0.0;
    };

    struct Geometry
    {
        size_t id = 0;

        std::shared_ptr<gs::Shape2D> shape = nullptr;

        std::vector<Constraint> cons;
    };

private:
    ct2::Scene m_layout;

    std::map<std::shared_ptr<gs::Shape2D>, std::shared_ptr<Geometry>> m_shape2geo;

}; // ConsEditView

}