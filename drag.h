#pragma once
#include "input.h"
#include "cbasecontrol.h"
#include "menucfg.h"

using namespace input;

class cdraggable
{
public:
	cdraggable() {
		difx = 0; dify = 0;
		m_dragging = false;
	}

	bool can_drag(cbasecontrol* c) {
		return ((mouse_in_region(c->rx, c->ry, c->w, c->h) && m.active_control == nullptr) || m.active_control == c);
	}

	void create_drag(cbasecontrol* c) {

		if ((mouse_in_region(c->ox, c->oy, c->w, c->drag_h) && m.active_control == nullptr) || m.active_control == c) {
			if (key_down(VK_LBUTTON)) {
				if (!m_dragging) {
					difx = mouse_pos.x - c->ox;
					dify = mouse_pos.y - c->oy;
					m_dragging = true;
				}
				c->ox = mouse_pos.x - difx;
				c->oy = mouse_pos.y - dify;

				m.active_control = c;
			}
			else
			{
				m.active_control = nullptr;
				m_dragging = false;
			}
		}
	}

private:
	float difx, dify;
	bool m_dragging;
};