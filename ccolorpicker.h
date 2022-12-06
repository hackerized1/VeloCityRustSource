#pragma once
#include "cbasecontrol.h"
#include "input.h"
#include "menu.h"
#include "cbutton.h"
#include "csliderfloat.h"

using namespace input;

cwindow* create_colorpicker_window();

struct hsv
{
	float h, s, v;

	void multiply()
	{
		h *= 3.6f;
		s *= 100.f;
		v *= 100.f;
	}
};

color_t HSVtoRGB(hsv value) {
	if (value.h > 360 || value.h < 0 || value.s >100 || value.s < 0 || value.v>100 || value.v < 0) {
		return {};
	}
	float s = value.s / 100;
	float v = value.v / 100;
	float C = s * v;
	float X = C * (1 - abs(fmod(value.h / 60.0, 2) - 1));
	float m = v - C;
	float r, g, b;
	if (value.h >= 0 && value.h < 60) {
		r = C, g = X, b = 0;
	}
	else if (value.h >= 60 && value.h < 120) {
		r = X, g = C, b = 0;
	}
	else if (value.h >= 120 && value.h < 180) {
		r = 0, g = C, b = X;
	}
	else if (value.h >= 180 && value.h < 240) {
		r = 0, g = X, b = C;
	}
	else if (value.h >= 240 && value.h < 300) {
		r = X, g = 0, b = C;
	}
	else {
		r = C, g = 0, b = X;
	}
	color_t ret;
	ret.r = (r + m) * 255.f;
	ret.b = (b + m) * 255.f;
	ret.g = (g + m) * 255.f;

	return ret;
}

class ccolorbutton : public cbasecontrol
{
public:
	ccolorbutton(bool display_info = true) {
		m_window = create_colorpicker_window();
		m_display_info = display_info;
	}

	virtual void think() override {

		if (mouse_in_region(rx, ry, w, h)) {

			//keypress logic
			if (key_pressed(VK_LBUTTON)) {

				m_window->m_draw_outline = false;
				m_window->m_hidden = false;
				m_window->m_draggable = false;
				m_window->m_popup = true;
				m_window->ox = w::m_x - m_window->w / 2;
				m_window->oy = w::m_y + 1;

				//set invoker of the window to this control
				m_window->m_invoker = this;

				menu_windows::swap_window(m_window);
			}
		}
	}

	virtual void render() override {

		d::antialias(false);

		//main
		d::rect1(rx, ry, w, h, m_color, 255 , 1);

		//gradient fill
		d::gradient_v1(rx, ry, w, h, { mcolors::gradient_a, mcolors::gradient_b }, m.maxalpha_gradient, 0);

		//internal animation
		d::rect1(rx, ry, w, h, mcolors::hover, m_animstate_alpha, 1);

		//outline
		d::antialias(false);
		d::rect1(rx, ry, w, h, m_inside ? mcolors::outlinelighter : mcolors::outline, 255);
	}

	virtual void handle_callbacks() override {
		return;
	}

	virtual void handle_tooltips() override {
		if (m.active_control != nullptr && m.active_control != this) return;
		if (!m_atooltip_alpha) return;

		//above
		if(this->tooltip.size())
		{
			float mx = rx + w / 2;
			float my = ry - m.tooltippad - m.tooltip_extra_y;

			d::set_font(f::tahoma);
			auto dim = d::get_string_dim_w(this->tooltip.c_str());
			mx -= dim.w / 2;
			my -= dim.h;
			d::set_text_outline(false);
			d::set_text_vertical_centering(false);
			d::rect1(mx - m.tooltip_extra_x, my - m.tooltip_extra_y, dim.w + m.tooltip_extra_x * 2, dim.h + m.tooltip_extra_y * 2, mcolors::ctrlbck, m_atooltip_alpha, 1);
			d::string_w(rx + w / 2, my, DT_CENTER, color::white, m_atooltip_alpha, this->tooltip.c_str());
			d::rect1(mx - m.tooltip_extra_x, my - m.tooltip_extra_y, dim.w + m.tooltip_extra_x * 2, dim.h + m.tooltip_extra_y * 2, mcolors::outlinelighter, m_atooltip_alpha);
		}


		//below
		if(m_display_info)
		{
			float mx = rx + w / 2;
			float my = ry + h + m.tooltippad + m.tooltip_extra_y;

			d::set_font(f::tahoma);
			auto info = d::format_w(L"0x%08X\n[r%.f,g%.f,b%.f]", color2dx(m_color, m_alpha), m_color.r, m_color.g, m_color.b);

			auto dim = d::get_string_dim_w(info.c_str());
			mx -= dim.w / 2;
			d::set_text_outline(false);
			d::set_text_vertical_centering(false);
			d::rect1(mx - m.tooltip_extra_x, my - m.tooltip_extra_y, dim.w + m.tooltip_extra_x * 2, dim.h + m.tooltip_extra_y * 2, mcolors::ctrlbck, m_atooltip_alpha, 1);
			d::string_w(rx + w / 2, my, DT_CENTER, color::white, m_atooltip_alpha, info.c_str());
			d::rect1(mx - m.tooltip_extra_x, my - m.tooltip_extra_y, dim.w + m.tooltip_extra_x * 2, dim.h + m.tooltip_extra_y * 2, mcolors::outlinelighter, m_atooltip_alpha);
		}
	}

	virtual int whoami() override {
		return controls::ColorButton;
	}

	virtual void post_render() override {
		return;
	}

	virtual void post_think() override {
		return;
	}

	color_t get()
	{
		return m_color;
	}

	void set(const color_t& color)
	{
		m_color = color;
	}

	void set(DWORD color)
	{
		m_color = dx2color(color);
	}

	cwindow* m_window = nullptr;
	color_t m_color = color::white;
	float m_alpha = 255.f;
	bool m_display_info;
};

void colorpickerwindow_callback(cbasecontrol* sender) {

	cwindow* wnd = (cwindow*)sender;

	//check if the user has clicked outside our client area
	if (key_pressed(VK_LBUTTON) && !mouse_in_region(wnd->ox, wnd->oy, wnd->w, wnd->h))
	{
		//check if our window is on top
		//if not then handle_focusing at the start of render allready took care of this and switched the window that was clicked on
		if (menu_windows::get_active_window() == wnd)
		{
			//our window is still active so we need to handle it ourselves
			if (menu_windows::windowlist.size() >= 2)
			{
				menu_windows::swap_window(menu_windows::windowlist[menu_windows::windowlist.size() - 2]);
			}
		}

		//make our window hidden and return because user clicked outside
		wnd->m_hidden = true;
		return;
	}
}


class ccolorpickerwindow : public cwindow
{
public:
	virtual void post_think() override
	{
		if (this->m_hidden) return;

		if ((mouse_in_region(rx + picker_pad, ry + picker_pad, picker_w, picker_h) && m.active_control == nullptr) || m.active_control == this) {

			if (key_down(VK_LBUTTON))
			{
				m.active_control = this;
				last_spot.x = mouse_pos.x - (rx + picker_pad);
				last_spot.y = mouse_pos.y - (ry + picker_pad);
				last_spot.x = std::clamp(last_spot.x, 0.f, picker_w);
				last_spot.y = std::clamp(last_spot.y, 0.f, picker_h);
			}
			else if (((mouse_in_region(rx + picker_pad, ry + picker_pad, picker_w, picker_h) && m.active_control == nullptr) || m.active_control == this) && !key_down(VK_LBUTTON))
			{
				m.active_control = nullptr;
			}
		}

		{
			float huepos = ((csliderfloat*)this->m_controls[0])->get();

			hsv value;
			value.h = huepos;
			value.s = 1.f;
			value.v = 1.f;
			value.multiply();
			gradient_color = HSVtoRGB(value);

			if (this->m_invoker) {
				bool pressed = ((cbutton*)this->m_controls[2])->get();

				ccolorbutton* clrbutton = (ccolorbutton*)this->m_invoker;
				hsv value;
				value.h = huepos;
				value.s = std::clamp(last_spot.x / picker_w, 0.0f, 1.0f);
				value.v = 1.0f - std::clamp(last_spot.y / picker_h, 0.0f, 1.0f);
				value.multiply();

				auto converted = HSVtoRGB(value);

				if (pressed) {
					clrbutton->m_color = converted;
					clrbutton->m_alpha = ((csliderfloat*)this->m_controls[1])->get();
				}
				selected_color = converted;
			}
		}
	}

	virtual void post_render() override
	{
		if (this->m_hidden) return;

		//picker
		d::antialias(false);
		d::gradient_h1(rx + picker_pad, ry + picker_pad, picker_w, picker_h, { color::white,gradient_color },255, 255);
		d::gradient_v1(rx + picker_pad, ry + picker_pad, picker_w, picker_h, { color::black,color::black },0, 255);

		//cursor
		auto oldclip = d::get_clip();
		d::set_clip1(rx + picker_pad, ry + picker_pad, picker_w, picker_h);
		d::antialias(true);
		d::circle(rx + last_spot.x + picker_pad, ry + last_spot.y + picker_pad, 5, 20, color::white, 140);
		d::circle(rx + last_spot.x + picker_pad, ry + last_spot.y + picker_pad, 6, 20, color::black, 140);
		d::set_clip(oldclip);

		//outline
		d::antialias(false);
		d::rect1(rx + picker_pad, ry + picker_pad, picker_w, picker_h, mcolors::outlinelighter, 255);

		//color preview
		d::antialias(false);
		d::rect1(rx + picker_pad + picker_w + picker_pad, ry + picker_pad, 30, picker_h, selected_color, 255, 1);

		//outline
		d::rect1(rx + picker_pad + picker_w + picker_pad, ry + picker_pad, 30, picker_h, mcolors::outlinelighter, 255);
	}

	float picker_w = 200;
	float picker_h = 200;

	float picker_pad = 5;

	vector2 last_spot = { 0,0 };

	color_t selected_color = color::white;
	color_t gradient_color = color::white;
};

cwindow* create_colorpicker_window()
{
	static ccolorpickerwindow* ptr = nullptr;
	static bool once = false;
	if (!once)
	{
		ptr = new ccolorpickerwindow;
		ptr->ox = 0, ptr->oy = 0;
		ptr->w = 245, ptr->h = 250;
		ptr->m_hidden = true;
		ptr->add_callback(colorpickerwindow_callback);
		menu_windows::windowlist.push_back(ptr);

		{
			auto c = new csliderfloat(0, 100);
			c->m_parent = ptr;
			c->ox = 5;
			c->oy = 210;
			c->w = 200;
			c->h = 15;
			c->tooltip = L"hue";
			ptr->m_controls.push_back(c);
		}

		{
			auto c = new csliderfloat(0, 255);
			c->m_parent = ptr;
			c->ox = 5;
			c->oy = 230;
			c->w = 200;
			c->h = 15;
			c->m_value = 255;
			c->m_delta = c->w;
			c->tooltip = L"alpha";
			ptr->m_controls.push_back(c);
		}

		{
			auto c = new cbutton(L"set", L"");
			c->m_parent = ptr;
			c->ox = 210;
			c->oy = 210;
			c->w = 30;
			c->h = 35;
			ptr->m_controls.push_back(c);
		}

		once = true;
	}

	return ptr;
}