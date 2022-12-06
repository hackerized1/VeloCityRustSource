#pragma once
#include "cbasecontrol.h"
#include "input.h"
#include "drawing.h"

using namespace input;

class ccheckbox : public cbasecontrol
{
public:
	ccheckbox(const std::wstring& name)
	{
		m_name = name;
	}

	virtual void think() override
	{
		if (mouse_in_region(rx, ry, w, h) && key_pressed(VK_LBUTTON))
		{
			m_toggle = !m_toggle;
			this->m_animstate2 = 1;
		}
	}

	virtual void render() override
	{
		d::antialias(false);
		if (m_toggle) {
			//main
			d::rect_center(rx + m_pad, ry + h / 2, m_size, mcolors::ctrl, 255, 1);
			//gradient
			d::gradient_rect_center_v(rx + m_pad, ry + h / 2, m_size, { mcolors::gradient_a, mcolors::gradient_b }, m.maxalpha_gradient, 0);
		}

		//internal animation
		d::rect_center(rx + m_pad, ry + h / 2, m_size, mcolors::hover, m_animstate_alpha, 1);

		//outline
		d::rect_center(rx + m_pad, ry + h / 2, m_size, m_inside ? mcolors::outlinelighter : mcolors::outline, 255);

		//external anim
		if (m_animstate2_alpha) {
			//d::antialias(true);
			//d::rect_center(rx + m_pad, ry + h / 2, (m.maxalpha1 - m_animstate2_alpha) / 10, color::white, m_animstate2_alpha,1);
			//d::antialias(false);
		}

		//text
		d::set_font(f::tahoma);
		d::set_text_outline(false);
		d::set_text_vertical_centering(true);
		d::string_w(rx + m_pad * 2 + m.mpad, ry + h / 2, DT_LEFT, color::white, 255, m_name.c_str());
	}

	void handle_callbacks() override
	{
		for (const auto& callback : m_callbacks)
			callback(this);
	}

	virtual int whoami() override
	{
		return controls::Checkbox;
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

	void set(bool newvalue)
	{
		m_toggle = newvalue;
	}

	std::wstring m_name;
	bool m_toggle = false;
	int m_pad = 10;
	int m_size = 6;
};