#pragma once
#include "cbasecontrol.h"
#include "drawing.h"

namespace labelcolors
{
	color_t normal = color::white;
	color_t working = color::orange;
	color_t error = color::red;
	color_t success = color::green;
}

class clabel : public cbasecontrol
{
public:
	clabel(const std::wstring& string, DWORD positioning = DT_CENTER)
	{
		m_name = string;
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
			d::set_font(f::tahoma);
			d::set_text_outline(true);
			d::set_text_vertical_centering(true);
			
			switch (m_position)
			{
			case DT_LEFT:
			{
				d::string_w(rx, ry + h / 2, m_position, m_color, 255, this->m_name.c_str());
				break;
			}
			case DT_CENTER:
			{
				d::string_w(rx + w / 2, ry + h / 2, m_position, m_color, 255, this->m_name.c_str());
				break;
			}
			case DT_RIGHT:
			{
				d::string_w(rx + w, ry + h / 2, m_position, m_color, 255, this->m_name.c_str());
				break;
			}
			default:
				break;
			}
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
		return controls::Label;
	}

	virtual void post_render() override {
		return;
	}

	virtual void post_think() override {
		return;
	}

	void set(const std::wstring& string, const color_t& color = labelcolors::normal)
	{
		m_name = string;
		m_color = color;
	}

	std::wstring m_name;
	DWORD m_position;
	color_t m_color = labelcolors::normal;
};