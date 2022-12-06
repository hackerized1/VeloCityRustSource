#pragma once
#include "cbasecontrol.h"
#include "input.h"
#include "drawing.h"
#include <map>

using namespace input;

std::string keycode_to_str(int keycode)
{
	switch (keycode)
	{
	case VK_LBUTTON: return "LMB";
	case VK_RBUTTON: return "RMB";
	case VK_MBUTTON: return "MMB";
	default:
		break;
	}

	char buffer[32] = { 0 };
	unsigned int scancode = MapVirtualKey(keycode, 0);

	switch (keycode)
	{
		case VK_LEFT: case VK_UP: case VK_RIGHT: case VK_DOWN: // arrow keys
		case VK_PRIOR: case VK_NEXT: // page up and page down
		case VK_END: case VK_HOME:
		case VK_INSERT: case VK_DELETE:
		case VK_DIVIDE: // numpad slash
		case VK_NUMLOCK:
		{
			scancode |= 0x100; // set extended bit
			break;
		}
	}

	GetKeyNameTextA(MAKELONG(0, scancode), buffer, sizeof buffer);
	return std::string(buffer);
}

class ckeybind : public cbasecontrol
{
public:
	virtual void think() override
	{
		if (m_active) {

			//user pressed backspace then reset the keybind
			if (key_pressed(VK_BACK)) {
				m_keycode = 0;
				m_keyname = L"not bound";
				m_active = false;
			}

			if (m_active) {
				bool found = false;
				for (int i = 1; i < 255; i++)
				{
					if (key_pressed(i))
					{
						//found our key
						found = true;

						m_keycode = i;
						m_active = false;

						std::string name = keycode_to_str(i);
						m_keyname = std::wstring(name.begin(), name.end());
						break;
					}
				}

				if (!found) {
					m_keycode = 0;
					m_keyname = L"waiting for key";
				}
			}
		}

		if (mouse_in_region(rx, ry, w, h) && key_pressed(VK_LBUTTON)) {
			m_active = true;
		}
		else if(key_pressed(VK_LBUTTON)) {
			m_active = false;
		}
	}

	virtual void render() override
	{
		//main
		d::antialias(false);
		d::rect1(rx, ry, w, h, mcolors::ctrl, 255, 1);

		//gradient
		d::gradient_v1(rx, ry, w, h, { mcolors::gradient_a, mcolors::gradient_b }, m.maxalpha_gradient, 0);

		//internal animation
		d::rect1(rx, ry, w, h, mcolors::hover, m_animstate_alpha, 1);

		//outline
		d::rect1(rx, ry, w, h, m_inside || m_active ? mcolors::outlinelighter : mcolors::outline, 255);

		d::set_font(f::tahoma_3);
		d::set_text_outline(false);
		d::set_text_vertical_centering(true);

		if (m_active)
			d::string_w(rx + w / 2, ry + h / 2, DT_CENTER, color::white, 255, L"✱");
		else
			d::string_w(rx + w / 2, ry + h / 2, DT_CENTER, color::white, 255, L"🗝");
	}

	void handle_callbacks() override
	{
		for (const auto& callback : m_callbacks)
			callback(this);
	}

	virtual void handle_tooltips() override
	{
		//below
		if (this->m_keyname.size())
		{
			{
				float mx = rx + w / 2;
				float my = ry + h + m.tooltippad + m.tooltip_extra_y;

				d::set_font(f::tahoma);
				auto dim = d::get_string_dim_w(m_keyname.c_str());
				mx -= dim.w / 2;
				d::set_text_outline(false);
				d::set_text_vertical_centering(false);
				d::rect1(mx - m.tooltip_extra_x, my - m.tooltip_extra_y, dim.w + m.tooltip_extra_x * 2, dim.h + m.tooltip_extra_y * 2, mcolors::ctrlbck, m_atooltip_alpha, 1);
				d::string_w(rx + w / 2, my, DT_CENTER, color::white, m_atooltip_alpha, m_keyname.c_str());
				d::rect1(mx - m.tooltip_extra_x, my - m.tooltip_extra_y, dim.w + m.tooltip_extra_x * 2, dim.h + m.tooltip_extra_y * 2, mcolors::outlinelighter, m_atooltip_alpha);
			}
		}

		return;
	}

	virtual int whoami() override
	{
		return controls::KeyBind;
	}

	virtual void post_render() override {
		return;
	}

	virtual void post_think() override {
		return;
	}

	int get() {
		return m_keycode;
	}

	void set(int keycode) {
		m_keycode = keycode;
		std::string name = keycode_to_str(keycode);
		m_keyname = std::wstring(name.begin(), name.end());
	}

	bool m_active = false;
	std::wstring m_keyname = L"not bound";
	int m_keycode = 0;
};