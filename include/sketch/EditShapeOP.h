#pragma once

#include <draft2/EditShapeOP.h>

namespace ee0 { class WxStagePage; }

namespace sketch
{

class EditShapeOP : public draft2::EditShapeOP
{
public:
    EditShapeOP(ee0::WxStagePage& stage, const std::shared_ptr<pt0::Camera>& cam,
        draft2::EditView& view, float capture_threshold, uint32_t shape_type);

    virtual bool OnMouseLeftUp(int x, int y) override;
    virtual bool OnMouseRightDown(int x, int y) override;
    virtual bool OnMouseRightUp(int x, int y) override;

    virtual bool OnPopMenuSelected(int type) override;

private:
    ee0::WxStagePage& m_stage;

    sm::ivec2 m_right_down_pos;

}; // EditShapeOP

}