#pragma once

#include <drawing2/EditShapeOP.h>

namespace ee0 { class WxStagePage; }

namespace sketch
{

class EditShapeOP : public dw2::EditShapeOP
{
public:
    EditShapeOP(ee0::WxStagePage& stage, const std::shared_ptr<pt0::Camera>& cam,
        dw2::EditView& view, float capture_threshold, uint32_t shape_type);

    virtual bool OnMouseRightDown(int x, int y) override;
    virtual bool OnMouseRightUp(int x, int y) override;

    virtual bool OnPopMenuSelected(int type) override;

private:
    ee0::WxStagePage& m_stage;

    sm::ivec2 m_right_down_pos;

}; // EditShapeOP

}