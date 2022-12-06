#pragma once
#include "cbasecontrol.h"
#include "drawing.h"

class cgroupbox : public cbasecontrol
{
public:
	cgroupbox(const std::wstring& name = L"", DWORD positioning = DT_LEFT)
	{
		m_name = name;
		m_position = positioning;
	}

	virtual void think() override
	{
		return;
	}

	virtual void render() override
	{
		if (m_name.size())
		{
			d::antialias(false);
			d::set_font(f::tahoma_1);
			d::set_text_outline(true);
			d::set_text_vertical_centering(true);

			auto dim = d::get_string_dim_w(m_name.c_str());

			switch (m_position)
			{
			case DT_LEFT:
			{
				d::string_w(rx + m_pad , ry, m_position, color::white, 255, this->m_name.c_str());
				d::group_box(rx, ry, w, h, rx + m_pad - 3, rx + m_pad + dim.w, mcolors::outlinelighter, 255);
				break;
			}
			case DT_CENTER:
			{
				d::string_w(rx + w / 2, ry, m_position, color::white, 255, this->m_name.c_str());
				d::group_box(rx, ry, w, h,
					rx + (w / 2) - (dim.w / 2) - 3,
					rx + (w / 2) + (dim.w / 2), mcolors::outlinelighter, 255);
				break;
			}
			case DT_RIGHT:
			{
				d::string_w(rx + w - m_pad, ry, m_position, color::white, 255, this->m_name.c_str());
				d::group_box(rx, ry, w, h, rx + w - m_pad - dim.w - 3, rx + w - m_pad, mcolors::outlinelighter, 255);
				break;
			}
			default:
				break;
			}
		}
		else {
			d::rect1(rx, ry, w, h, mcolors::outlinelighter, 255);
		}
	}

	void handle_callbacks() override
	{
		for (const auto& callback : m_callbacks)
			callback(this);
	}

	virtual void handle_animations() override
	{
		return;
	}

	virtual void handle_tooltips() override
	{
		return;
	}

	virtual int whoami() override
	{
		return controls::GroupBox;
	}

	virtual void post_render() override {
		return;
	}

	virtual void post_think() override {
		return;
	}

	std::wstring m_name;
	float m_pad = 5.f;
	DWORD m_position;
};