#pragma once

#include "sketch/ConstriantsType.h"

#include <ee0/RightPopupMenu.h>

namespace ee0 { class WxStagePage; }
namespace gs { class Shape2D; }

namespace sketch
{

class ConsEditView;

class RightPopupMenu : public ee0::RightPopupMenu
{
public:
    RightPopupMenu(ee0::WxStagePage* stage, ConsEditView& view);

    virtual void SetRightPopupMenu(wxMenu& menu, int x, int y) override;
    virtual void OnRightPopupMenu(int id) override;

private:
    void GetAllShapes(std::vector<std::shared_ptr<gs::Shape2D>>& shapes) const;

    static bool HaveConstriants(const std::vector<std::shared_ptr<gs::Shape2D>>& shapes, ConstriantsType type);

private:
    ee0::WxStagePage* m_stage = nullptr;

    ConsEditView& m_view;

}; // RightPopupMenu

}