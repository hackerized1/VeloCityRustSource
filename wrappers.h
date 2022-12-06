#pragma once
#include "cbasecontrol.h"
#include "cbutton.h"
#include "ccheckbox.h"
#include "cgroupbox.h"
#include "clabel.h"
#include "csliderfloat.h"
#include "ctab.h"
#include "ctabcontrol.h"
#include "ctextbox.h"
#include "cwindow.h"

#include "menu.h"

#include <vector>
#include "clistbox.h"
#include "ckeybind.h"
#include "fnv.h"
#include "cpicture.h"
#include "ccombobox.h"
#include "ccolorpicker.h"

struct control_metadata
{
	cbasecontrol* control = nullptr;
	std::string variable_name = "";
	int variable_name_hash = 0;
};

using callstack_callback_add_t = void(*)(control_metadata metadata);

//variables for the callstack
namespace vars
{
	cbasecontrol* current_parent = nullptr;
	std::string current_variable = "default";
	int current_var_hash = 0;

	//switch off the callstack
	bool m_state = true;

	//callstack
	std::vector<control_metadata> callstack;

	//callbacks to adding and removing items from the callstack
	std::vector<callstack_callback_add_t> add_callbacks;

	void callstack_add(cbasecontrol* c) {
		if (m_state == false) return;

		control_metadata a = { 0 };
		a.control = c;
		a.variable_name = current_variable;
		a.variable_name_hash = current_var_hash;
		callstack.push_back(a);

		for (const auto& callback : add_callbacks)
			callback(a);
	}

	//dimensions
	float ox = 0;
	float oy = 0;
	float w = 0;
	float h = 0;

	void set(cbasecontrol* control)
	{
		control->ox = vars::ox;
		control->oy = vars::oy;
		control->w = vars::w;
		control->h = vars::h;

		control->m_parent = current_parent;

		//if we are a window we dont need parenting control
		if (control->whoami() == controls::Window)
		{
			menu_windows::windowlist.push_back((cwindow*)control);
			callstack_add(control);
			return;
		}

		//choose where to add based on parent
		switch (current_parent->whoami())
		{
		case controls::Window:
		{
			if (control->whoami() != controls::Tab)
			{
				auto ptr = (cwindow*)current_parent;
				ptr->m_controls.push_back(control);

				callstack_add(control);
			}
			else {
				//printf("cant add this control to this parent!\n");
			}

			break;
		}
		case controls::Tab:
		{
			auto ptr = (ctab*)current_parent;
			ptr->m_controls.push_back(control);

			callstack_add(control);

			break;
		}
		case controls::TabControl:
		{
			auto ptr = (ctabcontrol*)current_parent;
			ptr->m_tabs.push_back((ctab*)control);

			callstack_add(control);

			break;
		}
		default:
			//printf("set defaulted, control parent is nonexistant\n");
			return;
		}
	}
}

namespace menu
{
	void set_parent(cbasecontrol* parent)
	{
		vars::current_parent = parent;
	}

	void set_variable_hash(int hash)
	{
		vars::current_var_hash = hash;
	}


#ifndef NDEBUG
	void set_variable(const std::string& name)
	{
		vars::current_variable = name;
	}
#else
	void set_variable(std::string a)
	{

	}
#endif

	void set_position(float ox, float oy, float w, float h)
	{
		vars::ox = ox;
		vars::oy = oy;
		vars::w = w;
		vars::h = h;
	}

	cbutton* button(const std::wstring& text)
	{
		cbutton* t = new cbutton(text);
		vars::set(t);
		return t;
	}

	ccheckbox* checkbox(const std::wstring& text)
	{
		auto t = new ccheckbox(text);
		vars::set(t);
		return t;
	}

	cgroupbox* groupbox(const std::wstring& text)
	{
		auto t = new cgroupbox(text);
		vars::set(t);
		return t;
	}

	clabel* label(const std::wstring& text)
	{
		auto t = new clabel(text);
		vars::set(t);
		return t;
	}

	csliderfloat* sliderfloat(float min, float max)
	{
		auto t = new csliderfloat(min, max);
		vars::set(t);
		return t;
	}

	ctab* tab(const std::wstring& text)
	{
		auto t = new ctab(text);
		vars::set(t);
		return t;
	}

	ctabcontrol* tabcontrol()
	{
		auto t = new ctabcontrol;
		vars::set(t);
		return t;
	}

	ctextbox* textbox()
	{
		auto t = new ctextbox;
		vars::set(t);
		return t;
	}

	cwindow* window(const std::wstring& title)
	{
		auto t = new cwindow(title);
		vars::set(t);
		return t;
	}

	ccolorbutton* colorpicker()
	{
		auto t = new ccolorbutton();
		vars::set(t);
		return t;
	}

	clistbox* listbox()
	{
		auto t = new clistbox();
		vars::set(t);
		return t;
	}

	ckeybind* keybind()
	{
		auto t = new ckeybind();
		vars::set(t);
		return t;
	}

	cpicture* picture()
	{
		auto t = new cpicture();
		vars::set(t);
		return t;
	}

	ccombobox* combobox()
	{
		auto t = new ccombobox();
		vars::set(t);
		return t;
	}
}

namespace serialization
{
	bool release_mode = false;

	std::string find_parent_name(const control_metadata& metadata)
	{
#ifndef NDEBUG
		if (!metadata.control->m_parent)
			return "";
		for (const auto& entry : vars::callstack)
		{
			//we cant be our own parent
			if (entry.control == metadata.control)
				continue;

			if (entry.control == metadata.control->m_parent)
				return entry.variable_name;
		}

		return "unknown";
#else
		return "";
#endif
	}

#ifndef NDEBUG
	void print_parent(const control_metadata& metadata)
	{
		printf("set_parent(%s);\n", find_parent_name(metadata).c_str());
	}

	void print_position(const control_metadata& metadata)
	{
		printf("set_position(%i, %i, %i, %i);\n", metadata.control->ox, metadata.control->oy, metadata.control->w, metadata.control->h);
	}

	void print_variable(const control_metadata& metadata)
	{
		printf("set_variable_hash(0x%x);\n", fnv::hash_runtime(metadata.variable_name.c_str()));
		if (release_mode == true)
		{
			printf("//set_variable(\"%s\");\n", metadata.variable_name.c_str());
		}
		else {
			printf("set_variable(\"%s\");\n", metadata.variable_name.c_str());
		}
	}

	void print_base(const control_metadata& metadata)
	{
		print_variable(metadata);
		print_parent(metadata);
		print_position(metadata);
	}

	void print_tooltip(const control_metadata& metadata)
	{
		if (metadata.control->tooltip.size())
			printf("%s->tooltip = L\"%ls\";\n", metadata.variable_name.c_str(), metadata.control->tooltip.c_str());
	}

	void button(const control_metadata& m)
	{
		auto c = (cbutton*)m.control;

		print_base(m);
		printf("%s = button(L\"%ls\");\n", m.variable_name.c_str(), c->m_name.c_str());
		print_tooltip(m);
	}

	void checkbox(const control_metadata& m)
	{
		auto c = (ccheckbox*)m.control;

		print_base(m);
		printf("%s = checkbox(L\"%ls\");\n", m.variable_name.c_str(), c->m_name.c_str());
		print_tooltip(m);
	}

	void groupbox(const control_metadata& m)
	{
		auto c = (cgroupbox*)m.control;

		print_base(m);
		printf("%s = groupbox(L\"%ls\");\n", m.variable_name.c_str(), c->m_name.c_str());
	}

	void label(const control_metadata& m)
	{
		auto c = (clabel*)m.control;

		print_base(m);
		printf("%s = label(L\"%ls\");\n", m.variable_name.c_str(), c->m_name.c_str());
		print_tooltip(m);
	}

	void sliderfloat(const control_metadata& m)
	{
		auto c = (csliderfloat*)m.control;

		print_base(m);
		printf("%s = sliderfloat(%f, %f);\n", m.variable_name.c_str(), c->m_min, c->m_max);
		print_tooltip(m);
	}

	void tab(const control_metadata& m)
	{
		auto c = (ctab*)m.control;

		print_variable(m);
		print_parent(m);
		printf("%s = tab(L\"%ls\");\n", m.variable_name.c_str(), c->m_name.c_str());
		print_tooltip(m);
	}

	void tabcontrol(const control_metadata& m)
	{
		auto c = (ctabcontrol*)m.control;

		print_base(m);
		printf("%s = tabcontrol();\n", m.variable_name.c_str());
		print_tooltip(m);
	}

	void textbox(const control_metadata& m)
	{
		auto c = (ctextbox*)m.control;

		print_base(m);
		printf("%s = textbox();\n", m.variable_name.c_str());
		printf("%s->m_hint = \"%s\";\n", m.variable_name.c_str(), c->m_hint.c_str());
		printf("%s->m_hidetext = %i;\n", m.variable_name.c_str(), c->m_hidetext);
		print_tooltip(m);
	}

	void window(const control_metadata& m)
	{
		auto c = (cwindow*)m.control;

		print_variable(m);
		print_position(m);
		printf("%s = window(L\"%ls\");\n", m.variable_name.c_str(), c->m_title.c_str());
		print_tooltip(m);
		printf("%s->m_disabled = %i;\n", m.variable_name.c_str(), c->m_disabled);
		printf("%s->m_draggable = %i;\n", m.variable_name.c_str(), c->m_draggable);
		printf("%s->m_closable = %i;\n", m.variable_name.c_str(), c->m_closable);
		printf("%s->m_popup = %i;\n", m.variable_name.c_str(), c->m_popup);
		printf("%s->m_draw_outline = %i;\n", m.variable_name.c_str(), c->m_draw_outline);
	}

	void colorpicker(const control_metadata& m)
	{
		auto c = (ccolorbutton*)m.control;

		print_base(m);
		printf("%s = colorpicker();\n", m.variable_name.c_str());
		print_tooltip(m);
		printf("%s->m_color = color_t{ %i,%i,%i };\n", m.variable_name.c_str(), (int)c->get().r, (int)c->get().g, (int)c->get().b);
	}

	void listbox(const control_metadata& m)
	{
		auto c = (clistbox*)m.control;
		print_base(m);
		printf("%s = listbox();\n", m.variable_name.c_str());
		printf("%s->m_canselectmultiple = %i;\n", m.variable_name.c_str(), c->m_canselectmultiple);
	}

	void keybind(const control_metadata& m)
	{
		auto c = (ckeybind*)m.control;
		print_base(m);
		printf("%s = keybind();\n", m.variable_name.c_str());
		printf("%s->set(%i);\n", m.variable_name.c_str(), c->m_keycode);
	}

	void picture(const control_metadata& m)
	{
		auto c = (cpicture*)m.control;
		print_base(m);
		printf("%s = picture();\n", m.variable_name.c_str());
	}

	void combobox(const control_metadata& m)
	{
		auto c = (ccombobox*)m.control;
		print_base(m);
		printf("%s = combobox();\n", m.variable_name.c_str());
	}

	std::string get_name(const control_metadata& metadata)
	{
		switch (metadata.control->whoami())
		{
		case controls::Button: return "cbutton";
		case controls::Checkbox: return "ccheckbox";
		case controls::GroupBox: return "cgroupbox";
		case controls::Label: return "clabel";
		case controls::SliderFloat: return "csliderfloat";
		case controls::Tab: return "ctab";
		case controls::TabControl: return "ctabcontrol";
		case controls::TextBox: return "ctextbox";
		case controls::Window: return "cwindow";
		case controls::ColorButton: return "ccolorbutton";
		case controls::ListBox: return "clistbox";
		case controls::KeyBind: return "ckeybind";
		case controls::Picture: return "cpicture";
		case controls::ComboBox: return "ccombobox";
		default:
			return "<control name not found>";
			break;
		}
	}

	void dump(bool hide_vars)
	{
		release_mode = hide_vars;

		//control definitions
		printf("namespace ui\n{\n");
		for (const auto& entry : vars::callstack)
		{
			printf("	%s* %s = nullptr;\n", get_name(entry).c_str(), entry.variable_name.c_str());
		}
		printf("}\n\n");

		//control settings and layout
		printf("void build_ui()\n{\nusing namespace ui;\nusing namespace menu;\n\n");
		for (const auto& entry : vars::callstack)
		{
			switch (entry.control->whoami())
			{
			case controls::Button: button(entry); break;
			case controls::Checkbox: checkbox(entry); break;
			case controls::GroupBox: groupbox(entry); break;
			case controls::Label: label(entry); break;
			case controls::SliderFloat: sliderfloat(entry); break;
			case controls::Tab: tab(entry); break;
			case controls::TabControl: tabcontrol(entry); break;
			case controls::TextBox: textbox(entry); break;
			case controls::Window: window(entry); break;
			case controls::ColorButton: colorpicker(entry); break;
			case controls::ListBox: listbox(entry); break;
			case controls::KeyBind: keybind(entry); break;
			case controls::Picture: picture(entry); break;
			case controls::ComboBox: combobox(entry); break;
			default:
				printf("tried dumping an unknown control type, ignoring..\n");
				break;
			}

			printf("\n");
		}
		printf("	for (const auto& ctrl : vars::callstack) ctrl.control->think();\n");
		printf("}\n");

		release_mode = false;
	}

#endif
}