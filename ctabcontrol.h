#pragma once
#include "cbasecontrol.h"
#include "drawing.h"
#include "input.h"
#include <vector>
#include "ctab.h"
#include "menu.h"

using namespace input;

class ctabcontrol : public cbasecontrol
{
public:
	virtual void think() override
	{
		if (m_tabs.size())
		{
			//if clicked within our tab height see what tab clicked on
			if (mouse_in_region(rx, ry, w, m_tabheight) && key_pressed(VK_LBUTTON))
			{
				float width = w / m_tabs.size();
				for (int i = 0; i < m_tabs.size(); i++)
				{
					if (mouse_in_region(rx + i * width, ry, width, m_tabheight))
					{
						m_activetabid = i;
					}
				}
			}

			//if the tab is out of bounds (probably removed externally)
			if (m_activetabid > m_tabs.size() - 1)
			{
				//set it to the last one
				m_activetabid = m_tabs.size() - 1;
			}
			else {
				//if active tab id is valid then process input
				auto tab = m_tabs[m_activetabid];

				tab->think();
				tab->post_think();
			}
		}
	}

	virtual void render() override
	{
		//background
		d::antialias(false);

		auto size = m_tabs.size();
		
		if (size)
		{
			int width = w / size;
			for (int i = 0; i < size; i++)
			{
				if (m_activetabid == i)
				{
					//tab
					d::rect1(rx + i * width, ry, width, m_tabheight, mcolors::ctrlbck, 255, 1);
					d::gradient_v1(rx + i * width, ry, width, m_tabheight, { mcolors::gradient_a, mcolors::gradient_b }, m.maxalpha_gradient, 0);

					//outline
					d::rect1(rx + i * width, ry, width, m_tabheight, mcolors::outline, 255);

					//background
					d::rect1(rx, ry + m_tabheight, w, h - m_tabheight, mcolors::ctrlbck, 255, 1);
				}
				else {
					//d::rect1(rx + i * width, ry, width, 15, mcolors::bck, 255, 1);
					//d::gradient_v1(rx + i * width, ry, width, 15, { color::black,color::black }, m.maxalpha_gradient, 0);
				}

				//tab name
				d::set_font(f::tahoma_1);
				d::set_text_vertical_centering(true);
				d::set_text_outline(false);
				d::string_w(rx + i * width + width / 2, ry + m_tabheight / 2, DT_CENTER, color::white, m_activetabid == i ? 255 : 150, m_tabs[i]->m_name.c_str());
			}
		}

		//outline of client area
		d::rect1(rx, ry + m_tabheight, w, h - m_tabheight, mcolors::outline, 150);

		//outline of whole box area
		//d::rect1(rx, ry, w, h, mcolors::outline, 150);

		if (size)
		{
			int width = w / size;

			//bottom filler to connect tab bottom to client area
			d::rect1(rx + m_activetabid * width + 1, ry + m_tabheight - 1, width - 2, 2, mcolors::ctrlbck, 255, 1);
		}

		if(size)
		{
			auto active_tab = m_tabs[m_activetabid];

			for (const auto& tab : m_tabs)
			{
				//anchor the actual tab control (position in this case is not used but it fixes some of the parenting problems)
				//update the anchor point
				tab->ax = this->ax + this->ox;
				tab->ay = this->ay + this->oy;
				tab->w = this->w;
				tab->h = this->h;
				tab->ox = 0; tab->oy = 0;

				//call to update render position (not used but might be for external use)
				tab->setup_draw();

				//set the anchor point to this controls rx ry
				for (const auto& ctrl : tab->m_controls)
				{
					//update the anchor point
					ctrl->ax = this->ax + this->ox;
					ctrl->ay = this->ay + this->oy;

					//update control position
					ctrl->setup_draw();

					//only render if active
					if (tab == active_tab)
					{
						//draw control
						ctrl->render();
						ctrl->post_render();
					}
				}

				//only do these for the active tab if the tabcontrols parent window is the active window (need to fix this globally)
				if (this->m_parent == menu_windows::get_active_window())
				{
					if (tab == active_tab)
					{
						for (const auto& ctrl : tab->m_controls)
						{
							ctrl->handle_animations();
						}

						for (const auto& ctrl : tab->m_controls)
						{
							ctrl->handle_tooltips();
						}
					}
				}
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
		return controls::TabControl;
	}

	virtual void post_render() override {
		return;
	}

	virtual void post_think() override {
		return;
	}

	int m_tabheight = 15;
	std::vector<ctab*> m_tabs;
	int m_activetabid = 0;
};