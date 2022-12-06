#pragma once
#include "drawing.h"
#include "cbasecontrol.h"
#include "input.h"
#include "cwindow.h"
#include "menu.h"
#include "clistbox.h"

cwindow* create_combobox_window();

class ccombobox : public cbasecontrol
{
public:
	ccombobox(bool multiselect = false) {
		m_window = create_combobox_window();
		m_listbox = new clistbox();
		m_listbox->m_canselectmultiple = multiselect;
	}

	virtual void think() override {

		//check if atleast one item is selected
		if (!m_listbox->m_canselectmultiple && m_listbox->m_items.size())
		{
			bool found = false;
			for (auto& item : m_listbox->m_items)
			{
				if (item.toggle == true)
				{
					found = true;
					break;
				}
			}

			if (!found) m_listbox->m_items.begin()->toggle = true;
		}

		if (mouse_in_region(rx, ry, w, h)) {

			//keypress logic
			if (key_pressed(VK_LBUTTON)) {
				//set the list items of the window to our items
				clistbox* otherlist = (clistbox*)(m_window->m_controls[0]);
				otherlist->m_items = this->m_listbox->m_items;
				otherlist->m_canselectmultiple = m_listbox->m_canselectmultiple;

				m_window->m_draw_outline = false;
				m_window->m_hidden = false;
				m_window->m_draggable = false;
				m_window->m_popup = true;
				m_window->ox = rx;
				m_window->oy = ry + h;
				m_window->w = w;
				m_window->h = otherlist->m_item_size * m_listbox->m_items.size();
				m_window->h = std::clamp(m_window->h, 16, INT_MAX);

				m_window->m_controls[0]->ox = 0;
				m_window->m_controls[0]->oy = 0;
				m_window->m_controls[0]->w = m_window->w;
				m_window->m_controls[0]->h = m_window->h;

				//set invoker of the window to this control
				m_window->m_invoker = this;

				menu_windows::swap_window(m_window);
			}
		}
	}

	virtual void render() override {

		d::antialias(false);

		//background
		d::rect1(rx, ry, w, h, mcolors::ctrlbck, 255, 1);

		//gradient
		d::gradient_v1(rx, ry, w, h, { mcolors::gradient_a, mcolors::gradient_b }, m.maxalpha_gradient, 0);

		//internal animation
		d::rect1(rx, ry, w, h, mcolors::hover, m_animstate_alpha, 1);

		//icon
		d::set_text_outline(false);
		d::set_font(f::tahoma_3);
		d::set_text_vertical_centering(true);

		if(m_flip_symbol == false)
			d::string_w(rx + w - 10, ry + h / 2 - 2, DT_CENTER, color::white, 255, L"🞃");
		else
			d::string_w(rx + w - 10, ry + h / 2 + 2, DT_CENTER, color::white, 255, L"🞁");

		//text
		d::set_font(f::tahoma);
		d::set_text_vertical_centering(true);
		auto active = this->m_listbox->get();
		std::string text;
		if (active.size())
		{
			if (m_listbox->m_canselectmultiple)
			{
				for (int i = 0; i < active.size(); i++)
				{
					text += active[i].name;

					auto dim = d::get_string_dim_a(text.c_str());
					if (dim.w > w - 5 - 20)
					{
						std::string copy = text;
						for (int z = 0; z < text.size(); z++)
						{
							auto d = d::get_string_dim_a(copy.c_str());
							if (d.w > w - 5 - 20)
							{
								copy.pop_back();
							}
							else {
								int chars = 3;
								if (copy.size() < 3) chars = copy.size();
								copy.replace(copy.end() - chars, copy.end(), "...");
								text = copy;
							}
						}

						break;
					}

					if(i != active.size() - 1) text += ",";
				}
			}
			else {
				text += active[0].name;
			}
		}

		d::string_a(rx + 5, ry + h / 2, DT_LEFT, color::white, 255, text.c_str());

		//outline
		d::antialias(false);
		d::rect1(rx, ry, w, h, m_inside ? mcolors::outlinelighter : mcolors::outline, 255);
	}

	virtual void handle_callbacks() override {
		for (const auto& callback : this->m_callbacks)
		{
			callback(this);
		}

		return;
	}

	virtual void handle_tooltips() override {
		return;
	}

	virtual int whoami() override {
		return controls::ComboBox;
	}

	virtual void post_render() override {
		return;
	}

	virtual void post_think() override {
		return;
	}

	//get all active items
	std::vector<listbox_entry_t> get()
	{
		std::vector<listbox_entry_t> out;

		for (const auto& entry : m_listbox->m_items)
		{
			if (entry.toggle == true)
				out.push_back(entry);
		}

		return out;
	}

	void push(const listbox_entry_t& item)
	{
		m_listbox->m_items.push_back(item);
	}

	void pop()
	{
		if(m_listbox->m_items.size())
			m_listbox->m_items.pop_back();
	}

	cwindow* m_window = nullptr;
	clistbox* m_listbox = nullptr;

	bool m_flip_symbol = false;
};

class ccomboboxwindow : public cwindow
{
public:
	virtual void post_think() override
	{
		if (this->m_hidden) return;
		if (this->m_invoker != 0)
		{
			auto cmb = (ccombobox*)(this->m_invoker);
			clistbox* ourlist = (clistbox*)(this->m_controls[0]);
			cmb->m_listbox->m_items = ourlist->m_items;
			cmb->handle_callbacks();

			//set the flipped bool to true
			cmb->m_flip_symbol = true;
		}
	}

	virtual void post_render() override
	{
		if (this->m_hidden) return;
	}
};

void comboboxwindow_callback(cbasecontrol* sender) {

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

		//set the flipped bool to false
		auto combo = (ccombobox*)wnd->m_invoker;
		combo->m_flip_symbol = false;

		//make our window hidden and return because user clicked outside
		wnd->m_hidden = true;
		return;
	}
}

cwindow* create_combobox_window()
{
	static ccomboboxwindow* ptr = nullptr;
	static bool once = false;
	if (!once)
	{
		ptr = new ccomboboxwindow;
		ptr->ox = 0, ptr->oy = 0;
		ptr->w = 50, ptr->h = 50;
		ptr->m_hidden = true;
		ptr->add_callback(comboboxwindow_callback);
		menu_windows::windowlist.push_back(ptr);

		{
			auto c = new clistbox();
			c->m_parent = ptr;
			c->ox = 0;
			c->oy = 0;
			c->w = 50;
			c->h = 50;
			ptr->m_controls.push_back(c);
		}

		once = true;
	}

	return ptr;
}