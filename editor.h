#pragma once
#include "wrappers.h"

//callback when control was added
void on_item_add(control_metadata metadata);

int grid_size = 5;

//wrapper for clamping

void clamp_to_grid(int& x, int& y)
{
	int cx = x;
	int cy = y;

	cx -= cx % grid_size;
	cy -= cy % grid_size;

	x = cx;
	y = cy;
}

class cbasepoint
{
public:
};

cbasepoint* active_point = nullptr;

class cresizepoint : public cbasepoint
{
public:
	bool think(cbasecontrol* c)
	{
		x = c->rx + c->w;
		y = c->ry + c->h;

		if ((mouse_in_region(x, y, w, h) && active_point == nullptr) || active_point == this)
		{
			if (key_down(VK_LBUTTON)) {
				if (!dragging) {
					difx = mouse_pos.x - x;
					dify = mouse_pos.y - y;
					dragging = true;
				}

				if (c->whoami() != controls::Window)
				{
					int width = mouse_pos.x - c->rx;
					int height = mouse_pos.y - c->ry;
					clamp_to_grid(width, height);

					c->w = width;
					c->h = height;
				}
				else {
					int width = mouse_pos.x - c->ox;
					int height = mouse_pos.y - c->oy;
					clamp_to_grid(width, height);

					c->w = width;
					c->h = height;
				}

				c->w = std::clamp<int>(c->w, 0, 99999);
				c->h = std::clamp<int>(c->h, 0, 99999);

				active_point = this;
			}
			else {
				active_point = nullptr;
				dragging = false;
			}
		}

		return dragging;
	}

	void render(cbasecontrol* c)
	{
		x = c->rx + c->w;
		y = c->ry + c->h;

		d::rect1(x, y, w, h, color::lime, 255, 1);
		d::rect1(x, y, w, h, color::black, 255);
	}

	bool dragging = false;
	float x = 0;
	float y = 0;
	float w = 10;
	float h = 10;

	float difx = 0;
	float dify = 0;
};

class cdraggablepoint : public cbasepoint
{
public:
	bool think(cbasecontrol* c)
	{
		x = c->rx;
		y = c->ry;
		w = c->w;
		h = c->h;

		if ((mouse_in_region(x, y, w, h) && active_point == nullptr) || active_point == this)
		{
			if (key_down(VK_LBUTTON)) {
				if (!dragging) {
					difx = mouse_pos.x - x;
					dify = mouse_pos.y - y;
					dragging = true;
				}

				if (c->whoami() != controls::Window)
				{
					const auto p = c->m_parent;

					int cx = mouse_pos.x;
					int cy = mouse_pos.y;

					c->ox = cx - difx - p->rx;
					c->oy = cy - dify - p->ry;

					clamp_to_grid(c->ox, c->oy);
				}
				else {
					c->ox = mouse_pos.x - difx;
					c->oy = mouse_pos.y - dify;
				}

				active_point = this;
			}
			else {
				active_point = nullptr;
				dragging = false;
			}
		}

		return dragging;
	}

	void render()
	{
		d::rect1(x, y, w, h, color::white, 255);
	}

	bool dragging = false;
	float x = 0;
	float y = 0;
	float w = 0;
	float h = 0;

	float difx = 0;
	float dify = 0;
};

class cdraginterface
{
public:
	void think(cbasecontrol* control)
	{
		move.think(control);
		resize.think(control);
		resize.render(control);
	}

	cdraggablepoint move;
	cresizepoint resize;
};

namespace editor
{
	int active_index = 0;
	int is_active = false;
	cdraginterface drag_interface;

	//current set parent
	cbasecontrol* current_parent = nullptr;

	//set varname
	std::string varname = "";
	std::string name = "";

	namespace ui
	{
		cwindow* hwindow = nullptr;
		clistbox* listbox1 = nullptr;
		cbutton* btn_set_parent = nullptr;
		cbutton* btn_delete = nullptr;
		cbutton* btn_button = nullptr;
		cbutton* btn_checkbox = nullptr;
		cbutton* btn_groupbox = nullptr;
		cbutton* btn_label = nullptr;
		cbutton* btn_slider = nullptr;
		cbutton* btn_tab = nullptr;
		cbutton* btn_tabcontrol = nullptr;
		cbutton* btn_textbox = nullptr;
		cbutton* btn_window = nullptr;
		cbutton* btn_colorpicker = nullptr;
		cbutton* btn_listbox = nullptr;
		cbutton* btn_keybind = nullptr;
		cbutton* btn_combobox = nullptr;
		cbutton* btn_picture = nullptr;
		cbutton* btn_apply = nullptr;
		ctextbox* in_varname = nullptr;
		ctextbox* in_name = nullptr;
	}

	std::string replace_space(const std::string& in)
	{
		std::string out = in;
		std::replace(out.begin(), out.end(), ' ', '_');
		return out;
	}

	void setup()
	{
#ifndef NDEBUG

		vars::add_callbacks.push_back(on_item_add);

		vars::m_state = false;

		using namespace ui;
		using namespace menu;

		set_variable("hwindow");
		set_position(59, 46, 250, 325);
		hwindow = window(L"window");
		hwindow->m_disabled = 0;
		hwindow->m_draggable = 1;
		hwindow->m_closable = 0;
		hwindow->m_popup = 0;
		hwindow->m_draw_outline = 1;

		set_variable("listbox1");
		set_parent(hwindow);
		set_position(5, 20, 115, 160);
		listbox1 = listbox();
		listbox1->m_canselectmultiple = 0;

		set_variable("btn_set_parent");
		set_parent(hwindow);
		set_position(125, 20, 120, 20);
		btn_set_parent = button(L"set parent");

		set_variable("btn_delete");
		set_parent(hwindow);
		set_position(125, 50, 120, 20);
		btn_delete = button(L"delete");

		set_variable("btn_button");
		set_parent(hwindow);
		set_position(10, 195, 70, 20);
		btn_button = button(L"button");

		set_variable("btn_checkbox");
		set_parent(hwindow);
		set_position(90, 195, 70, 20);
		btn_checkbox = button(L"checkbox");

		set_variable("btn_groupbox");
		set_parent(hwindow);
		set_position(170, 195, 70, 20);
		btn_groupbox = button(L"groupbox");

		set_variable("btn_label");
		set_parent(hwindow);
		set_position(10, 220, 70, 20);
		btn_label = button(L"label");

		set_variable("btn_slider");
		set_parent(hwindow);
		set_position(90, 220, 70, 20);
		btn_slider = button(L"slider");

		set_variable("btn_tab");
		set_parent(hwindow);
		set_position(170, 220, 70, 20);
		btn_tab = button(L"tab");

		set_variable("btn_tabcontrol");
		set_parent(hwindow);
		set_position(10, 245, 70, 20);
		btn_tabcontrol = button(L"tabcontrol");

		set_variable("btn_textbox");
		set_parent(hwindow);
		set_position(90, 245, 70, 20);
		btn_textbox = button(L"textbox");

		set_variable("btn_window");
		set_parent(hwindow);
		set_position(170, 245, 70, 20);
		btn_window = button(L"window");

		set_variable("btn_colorpicker");
		set_parent(hwindow);
		set_position(10, 270, 70, 20);
		btn_colorpicker = button(L"colorpicker");

		set_variable("btn_listbox");
		set_parent(hwindow);
		set_position(90, 270, 70, 20);
		btn_listbox = button(L"listbox");

		set_variable("btn_keybind");
		set_parent(hwindow);
		set_position(170, 270, 70, 20);
		btn_keybind = button(L"keybind");

		set_variable("btn_combobox");
		set_parent(hwindow);
		set_position(10, 295, 70, 20);
		btn_combobox = button(L"combobox");

		set_variable("btn_picture");
		set_parent(hwindow);
		set_position(90, 295, 70, 20);
		btn_picture = button(L"picture");

		set_variable("btn_apply");
		set_parent(hwindow);
		set_position(130, 160, 105, 20);
		btn_apply = button(L"apply");

		set_variable("in_varname");
		set_parent(hwindow);
		set_position(130, 80, 110, 20);
		in_varname = textbox();
		in_varname->m_hint = "var name";

		set_variable("in_name");
		set_parent(hwindow);
		set_position(130, 105, 110, 20);
		in_name = textbox();
		in_name->m_hint = "name";

		for (const auto& ctrl : vars::callstack) ctrl.control->think();

		vars::m_state = true;

#endif
	}

	void think()
	{
#ifndef NDEBUG

		if (key_pressed(VK_INSERT))
			is_active = !is_active;

		m.block_input = is_active;

		if (is_active)
		{
			if (key_pressed(VK_RIGHT))
				active_index++;
			else if (key_pressed(VK_LEFT))
				active_index--;

			if (active_index < 0)
			{
				active_index = vars::callstack.size() - 1;
			}
			else if (active_index > vars::callstack.size() - 1)
			{
				active_index = 0;
			}

			active_index = std::clamp<int>(active_index, 0, vars::callstack.size() - 1);

			const auto c = vars::callstack[active_index].control;
			drag_interface.think(c);
		}

		if (ui::btn_delete->get())
		{
			auto active = ui::listbox1->get();
			if (active.size())
			{
				for (int z = 0; vars::callstack.size(); z++)
				{
					auto item = vars::callstack[z];
					if (item.variable_name == active[0].name)
					{
						//found control in the callstack

						//delete it off the parent
						switch (item.control->whoami())
						{
						case controls::Window:
						{
							for (int i = 0; i < menu_windows::windowlist.size(); i++)
							{
								if (menu_windows::windowlist[i] == item.control)
								{
									//delete it off the window list
									menu_windows::windowlist.erase(menu_windows::windowlist.begin() + i);
								}
							}
							break;
						case controls::Tab:
						{
							ctabcontrol* tabctrlparent = (ctabcontrol*)item.control->m_parent;
							for (int i = 0; i < tabctrlparent->m_tabs.size(); i++)
							{
								if (tabctrlparent->m_tabs[i] == item.control)
								{
									tabctrlparent->m_tabs.erase(tabctrlparent->m_tabs.begin() + i);
									tabctrlparent->m_tabs.shrink_to_fit();
								}
							}
							break;
						}
						}
						default:

							switch (item.control->m_parent->whoami())
							{
							case controls::Tab:
							{
								ctab* tabparent = (ctab*)item.control->m_parent;
								for (int i = 0; i < tabparent->m_controls.size(); i++)
								{
									if (tabparent->m_controls[i] == item.control)
										tabparent->m_controls.erase(tabparent->m_controls.begin() + i);
								}

								break;
							}
							case controls::Window:
							{
								cwindow* windowparent = (cwindow*)item.control->m_parent;
								for (int i = 0; i < windowparent->m_controls.size(); i++) {
									if (windowparent->m_controls[i] == item.control)
										windowparent->m_controls.erase(windowparent->m_controls.begin() + i);
								}
								break;
							}
							default:
								break;
							}

							break;
						}

						//remove it off the callstack
						vars::callstack.erase(vars::callstack.begin() + z);

						//update our listbox
						ui::listbox1->m_items.clear();
						for (const auto& item : vars::callstack)
							ui::listbox1->m_items.push_back({ item.variable_name.c_str() });

						break;
					}
				}
			}
		}

		if (ui::btn_set_parent->get())
		{
			auto active = ui::listbox1->get();
			if (active.size())
			{
				for (int i = 0; vars::callstack.size(); i++)
				{
					if (vars::callstack[i].variable_name == active[0].name)
					{
						current_parent = vars::callstack[i].control;
						break;
					}
				}
			}
		}

		//if (ui::btn_apply->get())
		//{
		varname = ui::in_varname->get();
		name = ui::in_name->get();
		//}

		if (ui::btn_button->get() && current_parent != 0) {
			std::string var = "btn_" + replace_space(varname);
			menu::set_variable(var);
			menu::set_parent(current_parent);
			menu::set_position(5, 5, 100, 20);
			menu::button(std::wstring(name.begin(), name.end()));
		}

		if (ui::btn_checkbox->get() && current_parent != 0) {
			std::string var = "chk_" + replace_space(varname);
			menu::set_variable(var);
			menu::set_parent(current_parent);
			menu::set_position(5, 5, 100, 10);
			menu::checkbox(std::wstring(name.begin(), name.end()));
		}

		if (ui::btn_groupbox->get() && current_parent != 0) {
			std::string var = "grp_" + replace_space(varname);
			menu::set_variable(var);
			menu::set_parent(current_parent);
			menu::set_position(5, 5, 100, 100);
			menu::groupbox(std::wstring(name.begin(), name.end()));
		}

		if (ui::btn_label->get() && current_parent != 0) {
			std::string var = "label_" + replace_space(varname);
			menu::set_variable(var);
			menu::set_parent(current_parent);
			menu::set_position(5, 5, 100, 20);
			menu::label(std::wstring(name.begin(), name.end()));
		}

		if (ui::btn_slider->get() && current_parent != 0) {
			std::string var = "sliderf_" + replace_space(varname);
			menu::set_variable(var);
			menu::set_parent(current_parent);
			menu::set_position(5, 5, 100, 10);
			menu::sliderfloat(0, 100);
		}

		if (ui::btn_tab->get() && current_parent != 0) {
			std::string var = "tab_" + replace_space(varname);
			menu::set_variable(var);
			menu::set_parent(current_parent);
			menu::set_position(5, 5, 100, 30);
			menu::tab(std::wstring(name.begin(), name.end()));
		}

		if (ui::btn_tabcontrol->get() && current_parent != 0) {
			std::string var = "mtabctrl_" + replace_space(varname);
			menu::set_variable(var);
			menu::set_parent(current_parent);
			menu::set_position(5, 5, 100, 30);
			menu::tabcontrol();
		}

		if (ui::btn_textbox->get() && current_parent != 0) {
			std::string var = "in_" + replace_space(varname);
			menu::set_variable(var);
			menu::set_parent(current_parent);
			menu::set_position(5, 5, 100, 20);
			menu::textbox();
		}

		if (ui::btn_window->get()) {
			std::string var = "win_" + replace_space(varname);
			menu::set_variable(var);
			menu::set_position(5, 5, 100, 100);
			menu::window(std::wstring(name.begin(), name.end()));
			ui::btn_window->m_toggle = false;
		}

		if (ui::btn_colorpicker->get() && current_parent != 0) {
			std::string var = "clr_" + replace_space(varname);
			menu::set_variable(var);
			menu::set_parent(current_parent);
			menu::set_position(5, 5, 30, 10);
			menu::colorpicker();
		}

		if (ui::btn_listbox->get() && current_parent != 0) {
			std::string var = "list_" + replace_space(varname);
			menu::set_variable(var);
			menu::set_parent(current_parent);
			menu::set_position(5, 5, 100, 100);
			menu::listbox();
		}

		if (ui::btn_keybind->get() && current_parent != 0) {
			std::string var = "key_" + replace_space(varname);
			menu::set_variable(var);
			menu::set_parent(current_parent);
			menu::set_position(5, 5, 25, 20);
			menu::keybind();
		}

		if (ui::btn_picture->get() && current_parent != 0) {
			std::string var = "pic_" + replace_space(varname);
			menu::set_variable(var);
			menu::set_parent(current_parent);
			menu::set_position(5, 5, 50, 50);
			menu::picture();
		}

		if (ui::btn_combobox->get() && current_parent != 0) {
			std::string var = "combo_" + replace_space(varname);
			menu::set_variable(var);
			menu::set_parent(current_parent);
			menu::set_position(5, 5, 100, 20);
			menu::combobox();
		}

#endif
	}

	void render()
	{
#ifndef NDEBUG

		if (is_active)
		{
			d::set_font(f::tahoma);
			d::set_text_outline(true);
			d::set_text_vertical_centering(true);
			d::string_a(w::width / 2, 30, DT_CENTER, color::red, 255, "EDIT MODE");

			if (vars::callstack.size())
			{
				//get active control
				const auto m = vars::callstack[active_index];
				const auto c = vars::callstack[active_index].control;

				//control info
				d::set_text_vertical_centering(false);
				d::string_a(15, 15, DT_LEFT, color::white, 255, "%i/%i\n%s [%s]", active_index, vars::callstack.size() - 1, m.variable_name.c_str(), serialization::get_name(m).c_str());

				//outline
				d::rect1(c->rx, c->ry, c->w, c->h, color::lime, 255);
				//d::rect1(c->rx + 1, c->ry + 1, c->w - 2, c->h - 2, color::white, 255);
			}
		}
#endif // NDEBUG
	}
}

void on_item_add(control_metadata metadata)
{
	editor::ui::listbox1->m_items.push_back({ metadata.variable_name.c_str() });
}