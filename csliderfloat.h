#pragma once
#include "cbasecontrol.h"
#include "menucfg.h"
#include "input.h"
#include "drag.h"
#include "drawing.h"
#include <cmath>
#include <algorithm>

float inverse_lerp(float min, float max, float value)
{
	return (value - min) / (max - min);
}

class csliderfloat : public cbasecontrol, cdraggable
{
public:
	csliderfloat(float min, float max, const std::wstring& filter = L"%.f") {
		m_min = min;
		m_max = max;
		m_value = min;
		tooltip_filter = filter;
	}

	virtual void think() override {

		if (can_drag(this))
		{
			if (key_down(VK_LBUTTON))
			{
				m_delta = mouse_pos.x - (rx);
				m_delta = std::clamp(m_delta, 0, w);

				float percentage = 100.f / w;
				percentage *= m_delta;
				//set the value based on the delta
				m_value = std::lerp(m_min, m_max, percentage / 100.f);

				//set value of the callback if there is one
				if (m_callback != nullptr) *m_callback = m_value;

				//set this control to be active
				m.active_control = this;
			}
			else if (can_drag(this) && !key_down(VK_LBUTTON))
			{
				m.active_control = nullptr;
			}
		}
	}
	
	virtual void render() override {

		//background
		d::antialias(false);
		d::rect1(rx, ry + (m_size / 2), w, h - m_size, mcolors::ctrlbck, 255, 1);

		//main
		d::rect1(rx, ry + (m_size / 2), m_delta, h - m_size, mcolors::ctrl, 255, 1);
		d::gradient_v1(rx, ry + (m_size / 2), m_delta, h - m_size, { mcolors::gradient_a, mcolors::gradient_b }, m.maxalpha_gradient, 0);

		//tooltip
		if (!m_atooltip_alpha && this->tooltip.size()) {
			d::set_font(f::tahoma);
			d::set_text_outline(false);
			d::set_text_vertical_centering(true);
			d::string_w(rx + w / 2, ry + h / 2, DT_CENTER, color::white, 150, this->tooltip.c_str());
		}

		//internal animation
		d::rect1(rx, ry + (m_size / 2), w, h - m_size, mcolors::hover, m_animstate_alpha, 1);

		//outline
		d::rect1(rx, ry + (m_size / 2), w, h - m_size, m_inside ? mcolors::outlinelighter : mcolors::outline, 255);
	}

	void handle_tooltips() override
	{
		if (m.active_control != nullptr && m.active_control != this) return;
		if (!m_atooltip_alpha) return;

		//above
		{
			float mx = rx + w / 2;
			float my = ry - m.tooltippad - m.tooltip_extra_y;

			d::set_font(f::tahoma);
			auto value_str = d::format_w(tooltip_filter.c_str(), get());
			auto dim = d::get_string_dim_w(value_str.c_str());
			mx -= dim.w / 2;
			my -= dim.h;
			d::set_text_outline(false);
			d::set_text_vertical_centering(false);
			d::rect1(mx - m.tooltip_extra_x, my - m.tooltip_extra_y, dim.w + m.tooltip_extra_x * 2, dim.h + m.tooltip_extra_y * 2, mcolors::ctrlbck, m_atooltip_alpha, 1);
			d::string_w(rx + w / 2, my, DT_CENTER, color::white, m_atooltip_alpha, value_str.c_str());
			d::rect1(mx - m.tooltip_extra_x, my - m.tooltip_extra_y, dim.w + m.tooltip_extra_x * 2, dim.h + m.tooltip_extra_y * 2, mcolors::outlinelighter, m_atooltip_alpha);
		}

		//below
		if (this->tooltip.size())
		{
			{
				float mx = rx + w / 2;
				float my = ry + h + m.tooltippad + m.tooltip_extra_y;

				d::set_font(f::tahoma);
				auto dim = d::get_string_dim_w(this->tooltip.c_str());
				mx -= dim.w / 2;
				d::set_text_outline(false);
				d::set_text_vertical_centering(false);
				d::rect1(mx - m.tooltip_extra_x, my - m.tooltip_extra_y, dim.w + m.tooltip_extra_x * 2, dim.h + m.tooltip_extra_y * 2, mcolors::ctrlbck, m_atooltip_alpha, 1);
				d::string_w(rx + w / 2, my, DT_CENTER, color::white, m_atooltip_alpha, this->tooltip.c_str());
				d::rect1(mx - m.tooltip_extra_x, my - m.tooltip_extra_y, dim.w + m.tooltip_extra_x * 2, dim.h + m.tooltip_extra_y * 2, mcolors::outlinelighter, m_atooltip_alpha);
			}
		}
	}

	void handle_callbacks() override {
		return;
	}

	virtual int whoami() override {
		return controls::SliderFloat;
	}

	float get() {
		return m_value;
	}

	void set(float value)
	{
		m_value = value;

		//the holy grail
		float percentage = (abs(m_min) + value) / (abs(m_min) + m_max);
		m_delta = std::lerp(0, w, percentage);
	}

	virtual void post_render() override {
		return;
	}

	virtual void post_think() override {
		return;
	}

	float* m_callback = nullptr;

	float m_value;
	int m_size = 0;
	float m_min, m_max;
	int m_delta = 0;
};