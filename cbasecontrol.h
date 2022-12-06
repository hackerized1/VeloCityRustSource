#pragma once
#include <string>
#include "menucfg.h"
#include "clock.h"
#include "input.h"

class cwindow;
class cbasecontrol;

using callback_t = void(*)(cbasecontrol* sender);

class cbasecontrol
{
public:
	virtual void think() = 0;
	virtual void render() = 0;

	virtual void handle_animations()
	{
		this->m_inside = input::mouse_in_region(rx, ry, w, h);

		//increment timers
		if (this->m_inside || this == m.active_control)
		{
			if (this->m_animstate < 1.f) {
				this->m_animstate = std::clamp(m_animstate + m.aspeed * c::deltatime, 0.f, 1.f);
			}

			if (this->m_atooltip < 1.f) {
				this->m_atooltip = std::clamp(m_atooltip + m.aspeedtooltip * c::deltatime, 0.f, 1.f);
			}
		}
		else {
			this->m_animstate = std::clamp(m_animstate - m.aspeed * c::deltatime, 0.f, 1.f);
			this->m_atooltip = std::clamp(m_atooltip - m.aspeedtooltip * c::deltatime, 0.f, 1.f);
		}

		this->m_animstate2 = std::clamp(m_animstate2 - m.aspeed1 * c::deltatime, 0.f, 1.f);

		//set up alphas
		this->m_animstate_alpha = std::lerp(0.f, m.maxalpha, m_animstate);
		this->m_animstate2_alpha = std::lerp(0.f, m.maxalpha1, m_animstate2);
		this->m_atooltip_alpha = std::lerp(0.f, 255.f, m_atooltip);
	}

	virtual void handle_tooltips()
	{
		if (m.active_control != nullptr && m.active_control != this) return;
		if (!m_atooltip_alpha) return;

		if (this->tooltip.size())
		{
			float mx = rx + w / 2;
			float my = ry + h + m.tooltippad + m.tooltip_extra_y;

			d::set_font(f::tahoma);
			d::set_text_vertical_centering(false);
			d::set_text_outline(false);
			auto dim = d::get_string_dim_w(this->tooltip.c_str());
			mx -= dim.w / 2;
			d::rect1(mx - m.tooltip_extra_x, my - m.tooltip_extra_y, dim.w + m.tooltip_extra_x * 2, dim.h + m.tooltip_extra_y * 2, mcolors::ctrlbck, m_atooltip_alpha, 1);
			d::string_w(rx + w / 2, my, DT_CENTER, color::white, m_atooltip_alpha, this->tooltip.c_str());
			d::rect1(mx - m.tooltip_extra_x, my - m.tooltip_extra_y, dim.w + m.tooltip_extra_x * 2, dim.h + m.tooltip_extra_y * 2, mcolors::outlinelighter, m_atooltip_alpha);
		}
	}

	virtual void handle_callbacks() = 0;
	virtual void post_render() = 0;
	virtual void post_think() = 0;

	virtual int whoami() = 0;

	void setup_draw() {
		rx = ax + ox, ry = ay + oy;
	}

	float center_texty() {
		return ry + (h / 2.f) - m.textpady;
	}

	float center_textx() {
		return rx + (w / 2.f) - m.textpadx;
	}

	void add_callback(callback_t function) {
		m_callbacks.push_back(function);
	}

	//offset from menu x,y;
	int ax = 0.f, ay = 0.f;

	//position and dimensions
	int ox = 0, oy = 0;
	int w = 0.f, h = 0.f;
	int drag_h = 15.f;

	//postion for rendering
	int rx = 0, ry = 0;

	//internal animation
	float m_animstate = 0.f;
	float m_animstate_alpha = 0;

	//external animation
	float m_animstate2 = 0.f;
	float m_animstate2_alpha = 0;

	//tooltip animation
	float m_atooltip = 0.f;
	float m_atooltip_alpha = 0.f;

	bool m_pressed = false;
	bool m_inside = false;

	std::wstring tooltip = L"";
	std::wstring tooltip_filter = L"%ls";

	std::vector<callback_t> m_callbacks;
	cbasecontrol* m_parent;
};