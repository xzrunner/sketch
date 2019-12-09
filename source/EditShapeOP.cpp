#include "sketch/EditShapeOP.h"

#include <ee0/WxStagePage.h>
#include <ee0/RightPopupMenu.h>

#include <wx/menu.h>

namespace sketch
{

EditShapeOP::EditShapeOP(ee0::WxStagePage& stage, const std::shared_ptr<pt0::Camera>& cam,
                         dw2::EditView& view, float capture_threshold, uint32_t shape_type)
    : dw2::EditShapeOP(cam, view, capture_threshold, shape_type)
    , m_stage(stage)
{
    m_right_down_pos.MakeInvalid();
}

bool EditShapeOP::OnMouseRightDown(int x, int y)
{
	if (dw2::EditShapeOP::OnMouseRightDown(x, y)) {
		return true;
	}

    m_right_down_pos.Set(x, y);

	return false;
}

bool EditShapeOP::OnMouseRightUp(int x, int y)
{
    if (dw2::EditShapeOP::OnMouseRightUp(x, y)) {
        return true;
    }

    if (!m_right_down_pos.IsValid()) {
        return false;
    }

	if (m_right_down_pos == sm::ivec2(x, y))
	{
        auto popup = m_stage.GetImpl().GetPopupMenu();
        if (popup)
        {
            wxMenu menu;
            popup->SetRightPopupMenu(menu, x, y);
            m_stage.PopupMenu(&menu, x, y);
        }
	}

    m_right_down_pos.MakeInvalid();

    return false;
}

bool EditShapeOP::OnPopMenuSelected(int type)
{
    if (dw2::EditShapeOP::OnPopMenuSelected(type)) {
        return true;
    }

    auto popup = m_stage.GetImpl().GetPopupMenu();
    if (popup) {
        popup->OnRightPopupMenu(type);
    }

    return false;
}

}