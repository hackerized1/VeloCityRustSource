#pragma once
#include "cbasecontrol.h"
#include "input.h"
#include "drawing.h"
#include <vector>
#include <algorithm>

using namespace input;

struct listbox_entry_t
{
	std::string name;
	bool toggle = 0;
	int value = 0;
};

class clistbox : public cbasecontrol
{
public:
	clistbox()
	{
		m_canselectmultiple = false;
	}

	virtual void think() override
	{
		//check if atleast one item is selected
		if (!m_canselectmultiple && m_items.size())
		{
			bool found = false;
			for (auto& item : m_items)
			{
				if (item.toggle == true)
				{
					found = true;
					break;
				}
			}

			if (!found) m_items.begin()->toggle = true;
		}


		int height = m_items.size() * m_item_size;
		int delta = height - this->h;

		int slider_size = std::clamp(this->h - delta, 16, this->h);
		int sx = rx + w - m_slider_width;
		int sy = ry;
		int sw = m_slider_width;
		int sh = slider_size;

		//make sure the slider is never bigger than the actual client box
		m_offset = std::clamp<int>(m_offset, 0, h - slider_size);

		//scroll bar
		if (delta > 0)
		{
			if ((mouse_in_region(sx, sy + m_offset, sw, sh) && m.active_control == nullptr) || m.active_control == this) {
				if (key_down(VK_LBUTTON)) {
					if (!m_dragging) {
						m_dify = mouse_pos.y - sy - m_offset;
						m_dragging = true;
					}

					int half_slider_size = slider_size / 2;
					m_offset = mouse_pos.y - m_dify - ry;
					m_offset = std::clamp<int>(m_offset, 0, h - slider_size);

					m.active_control = this;
				}
				else
				{
					m.active_control = nullptr;
					m_dragging = false;
				}
			}
		}

		//item selection
		//if the mouse click was inside the control
		//prevents clicking when off client and there is items that are below or above
		if (mouse_in_region(rx, ry, w, h) && key_pressed(VK_LBUTTON))
		{
			for (int i = 0; i < m_items.size(); i++)
			{
				int rendery = ry - m_itemsoffset + (m_item_size * i);
				int width = delta > 0 ? this->w - m_slider_width : this->w;

				if (mouse_in_region(rx, rendery + 1, width, m_item_size - 2))
				{
					if (m_canselectmultiple)
						m_items[i].toggle = !m_items[i].toggle;
					else {
						for (auto& item : m_items)
							item.toggle = false;

						m_items[i].toggle = !m_items[i].toggle;
					}

					//callback
					handle_callbacks();
					break;
				}
			}
		}

	}

	virtual void render() override
	{
		int height = m_items.size() * m_item_size;

		int delta = height - this->h;

		int slider_size = std::clamp(this->h - delta, 16, this->h);
		int sx = rx + w - m_slider_width;
		int sy = ry;
		int sw = m_slider_width;
		int sh = slider_size;

		if (delta > 0)
		{
			int half_slider_size = slider_size / 2;
			int slider_min = ry + half_slider_size;
			int slider_max = ry + h - half_slider_size;

			//calculate the slider middle pos and then get the percentage
			int slider_target = ry + half_slider_size + m_offset;

			float percentage = (slider_max - slider_min) / 100.f;
			float value = (float)m_offset / percentage;

			//calulate move multiplier based on height diffrence
			float multiplier = height / (float)h;

			m_itemsoffset = std::lerp(0, delta, value / 100.f);
		}

		//main
		d::antialias(false);
		d::rect1(rx, ry, w, h, mcolors::ctrlbck, 255, 1);

		if (delta > 0)
		{
			//slider
			d::rect1(sx, sy + m_offset, sw, sh, mcolors::ctrl, 255, 1);
			d::gradient_v1(sx, sy + m_offset, sw, sh, { color::black, color::black }, m.maxalpha_gradient, 0);

			//slider client area outline
			d::rect1(sx, sy, m_slider_width, h, mcolors::outline, 255);

			//slider outline
			d::rect1(sx, sy + m_offset, sw, sh, mcolors::outline, 255);
		}

		auto oldclip = d::get_clip();
		d::set_clip1(rx, ry, w, h);

		d::set_font(f::tahoma);
		d::set_text_outline(false);
		d::set_text_vertical_centering(true);

		//items
		for (int i = 0; i < m_items.size(); i++)
		{
			int rendery = ry - m_itemsoffset + (m_item_size * i);

			//check if the item is inside
			if (rendery < ry - m_item_size)
				continue;

			if (rendery + m_item_size > ry + h + m_item_size)
				continue;

			int width = delta > 0 ? this->w - m_slider_width : this->w;
			d::rect1(rx, ry - m_itemsoffset + (m_item_size * i), width, m_item_size, m_items[i].toggle ? mcolors::ctrl : mcolors::ctrlbck, 255, 1);

			if(m_items[i].toggle)
				d::gradient_h1(rx, ry - m_itemsoffset + (m_item_size * i), width, m_item_size, { color::black, color::black }, m.maxalpha_gradient, 0);

			d::string_a(rx + 4, ry - m_itemsoffset + (m_item_size * i) + m_item_size / 2, DT_LEFT, color::white, 255, "%s\n", m_items[i].name.c_str());
		}

		d::set_clip(oldclip);

		//outline
		d::rect1(rx, ry, w, h, mcolors::outline, 255);
	}

	void handle_callbacks() override
	{
		for (const auto& callback : this->m_callbacks)
		{
			callback(this);
		}

		return;
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
		return controls::ListBox;
	}

	virtual void post_render() override {
		return;
	}

	virtual void post_think() override {
		return;
	}

	//clears all items
	void clear()
	{
		m_items.clear();
	}

	void push(const listbox_entry_t& item)
	{
		m_items.push_back(item);
	}

	void pop()
	{
		if(m_items.size())
			m_items.pop_back();
	}

	//removes an item from the list
	bool remove(const std::string& name)
	{
		for (int i = 0; i < m_items.size(); i++)
		{
			if (!m_items[i].name.compare(name))
			{
				m_items.erase(m_items.begin() + i);
				return true;
			}

			return false;
		}
	}

	//get all active items
	std::vector<listbox_entry_t> get()
	{
		std::vector<listbox_entry_t> out;

		for (const auto& entry : m_items)
		{
			if (entry.toggle == true)
				out.push_back(entry);
		}

		return out;
	}

	//scroll offset
	int m_offset = 0;
	//items offset
	float m_itemsoffset = 0;
	
	std::vector<listbox_entry_t> m_items;
	bool m_canselectmultiple = false;

	int m_item_size = 16;
	int m_slider_width = 6;

	//slider vars
	int m_dify = 0;
	bool m_dragging = false;
};