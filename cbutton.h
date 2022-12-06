#pragma once
#include "cbasecontrol.h"
#include "menucfg.h"
#include "drawing.h"
#include <algorithm>
#include <cmath>

using namespace input;

class cbutton : public cbasecontrol
{
public:
	cbutton(const std::wstring& name, const std::wstring& tooltip = L"")
	{
		m_name = name;
		this->tooltip = tooltip;
	}

	virtual void think() override
	{
		if (m_toggle)
			m_toggle = false;

		if (mouse_in_region(rx, ry, w, h))
		{
			//keypress logic
			if (key_pressed(VK_LBUTTON))
			{
				m_animstate2 = 1.f;
				m_last_click.x = mouse_pos.x - rx;
				m_last_click.y = mouse_pos.y - ry;
				m_toggle = !m_toggle;

				handle_callbacks();

				//if mouse down inside
				if (key_down(VK_LBUTTON))
					m_pressed = true;
				else
					m_pressed = false;
			}
			else if (!key_down(VK_LBUTTON)) {
				m_pressed = false;
			}
		}
		else
		{
			m_pressed = false;
		}
	}

	virtual void render() override
	{
		//main
		d::antialias(false);
		d::rect1(rx, ry, w, h, m_pressed ? mcolors::ctrlbck : mcolors::ctrl, 255, 1);
		d::gradient_v1(rx, ry, w, h, { mcolors::gradient_a, mcolors::gradient_b }, m.maxalpha_gradient, 0);

		//internal animation
		if (m_animstate_alpha) {
			float fill = std::lerp(ry + h, ry, m_animstate_alpha / m.maxalpha);
			d::rect(rx, fill, rx + w, ry + h, mcolors::hover, 150, 1);
		}

		//text rendering
		d::set_font(f::tahoma);
		d::set_text_outline(true);
		d::set_text_vertical_centering(true);
		d::string_w(rx + w / 2, ry + h / 2, DT_CENTER, color::white, 255, m_name.c_str());
		d::set_text_vertical_centering(false);

		if (m_animstate2_alpha) {
			auto oldclip = d::get_clip();
			d::set_clip1(rx, ry, w, h);
			d::antialias(true);
			d::circle(rx + m_last_click.x, ry + m_last_click.y, (m.maxalpha1 - m_animstate2_alpha), 20, color::gray , m_animstate2_alpha, 1);
			d::set_clip(oldclip);
		}

		//outline
		d::antialias(false);
		d::rect1(rx, ry, w, h, m_inside ? mcolors::outlinelighter : mcolors::outline, 255);
	}

	void handle_callbacks() override
	{
		for (const auto& callback : m_callbacks)
			callback(this);
	}

	virtual int whoami() override
	{
		return controls::Button;
	}

	virtual void post_render() override {
		return;
	}

	virtual void post_think() override {
		return;
	}

	bool get()
	{
		return m_toggle;
	}

public:
	input::vector2 m_last_click = { 0,0 };
	bool m_toggle = false;
	std::wstring m_name = L"";
};
