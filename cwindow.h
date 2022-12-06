#pragma once
#include "cbasecontrol.h"
#include "drag.h"
#include <vector>
#include "D2D1.h"

class cwindow : public cbasecontrol, cdraggable
{
public:
	cwindow() = default;
	cwindow(const std::wstring& title)
	{
		m_title = title;
	}
	cwindow(float x, float y, float width, float height)
	{
		this->ox = x, this->oy = y;
		this->w = width, this->h = height;
	}

	virtual void think() override {

		if (m_disabled || m_hidden)
			return;

		if (m_draggable) {
			create_drag(this);
		}

		//process input callback
		handle_callbacks();

		//process input for controls in the window
		for (const auto& control : m_controls) {
			control->think();
			control->post_think();
		}
	}

	virtual void render() override {
		if (m_hidden)
			return;

		//background
		d::antialias(false);
		d::rect1(ox, oy, w, h, mcolors::bck, 255, 1);

		//window gradient
		//d::gradient_v1(ox, oy, w, h, { color::black,color::black }, 100, 0);

		if (!m_popup)
		{
			//titlebar
			d::rect1(ox, oy, w, drag_h, mcolors::ctrl, 255, 1);
			d::gradient_v1(ox, oy, w, drag_h, { mcolors::gradient_a, mcolors::gradient_b }, m.maxalpha_gradient, 0);
			//outline
			d::rect1(ox, oy, w, drag_h, mcolors::outline, 255);

			//title
			if (m_title.size())
			{
				d::set_text_outline(true);
				d::set_font(f::tahoma_1);
				d::set_text_vertical_centering(true);
				d::string_w(ox + w / 2, oy + drag_h / 2, DT_CENTER, color::white, 255, m_title.c_str());
				d::set_text_vertical_centering(false);
			}
		}

		//window outline
		d::rect1(ox, oy, w, h, mcolors::ctrl, 150);
	}

	virtual void handle_callbacks() override {
		for (const auto& callback : m_callbacks)
			callback(this);
	}

	virtual void handle_animations() override {
		return;
	}

	virtual void handle_tooltips() override{
		return;
	}

	virtual int whoami() override {
		return controls::Window;
	}

	virtual void post_render() override {
		return;
	}

	virtual void post_think() override {
		return;
	}

	//window settings
	bool m_disabled = false;
	bool m_draggable = true;
	bool m_closable = false;
	bool m_popup = false;
	bool m_hidden = false;
	bool m_draw_outline = true;


	//input bypass
	bool m_bypasses_input = false;

	//window title
	std::wstring m_title = L"";

	//invokee of the window, mainly used to send back data to a color picker control
	cbasecontrol* m_invoker = nullptr;

	//list of controls
	std::vector<cbasecontrol*> m_controls;
};