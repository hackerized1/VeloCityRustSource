#pragma once
#include "cbasecontrol.h"
#include "input.h"
#include "drawing.h"

using namespace input;

bool capture_input(std::string& out)
{
	//get the key pressed
	for (int x = 32; x <= 107; ++x)
	{
		if (!key_pressed(x)) continue;

		//space
		if (x == 32)
		{
			out += (char)x;
			return true;
		}

		//numbers
		if (x > 47 && x < 58)
		{
			out += (char)x;
			return true;
		}
		else if (x > 95 && x < 107) { //numpad
			out += (char)(x - 48);
			return true;
		}

		//letters
		if (x > 64 && x < 91)
		{
			out += key_down(VK_SHIFT) ? (char)x : (char)(x + 32);
			return true;
		}
	}

	return false;
}

void remove_newlines(std::string& in)
{
	for (int i = in.size() - 1; i >= 0; i--)
	{
		if (in[i] == 10 || in[i] == 13)
			in.erase(in.begin() + i);
	}


}

class ctextbox : public cbasecontrol
{
public:
	virtual void think() override
	{
		if (mouse_in_region(rx, ry, w, h))
		{
			if (key_pressed(VK_LBUTTON))
			{
				m.active_control = this;
			}
			
			m_inside = true;
		}
		else {
			if (key_pressed(VK_LBUTTON))
			{
				if(m.active_control == this) m.active_control = nullptr;
			}

			m_inside = false;
		}

		if (m.active_control == this)
		{
			//delete
			if (key_pressed(VK_BACK))
			{
				if (m_text.size())
				{
					m_text.pop_back();
				}
			}
			else if (key_down(VK_BACK))
			{
				m_lastdelete += c::deltatime;

				if (m_lastdelete > m.wait_amount_between_delete)
				{
					if (!m_text.empty())
					{
						m_text.pop_back();
					}
					m_lastdelete = 0.f;
				}
			}
			else if (key_down(VK_CONTROL) && key_pressed(0x56)) //paste operation
			{
				OpenClipboard(NULL);
				auto handle = GetClipboardData(CF_TEXT);
				if (!handle)
				{
					CloseClipboard();
					return;
				}
				char* pszText = static_cast<char*>(GlobalLock(handle));
				this->m_text += pszText;
				GlobalUnlock(handle);
				CloseClipboard();

				remove_newlines(m_text);
			}
			else if (key_down(VK_CONTROL) && key_pressed(0x56)) //paste operation
			{
				OpenClipboard(NULL);
				auto handle = GetClipboardData(CF_TEXT);
				if (!handle)
				{
					CloseClipboard();
					return;
				}
				char* pszText = static_cast<char*>(GlobalLock(handle));
				this->m_text += pszText;
				GlobalUnlock(handle);
				CloseClipboard();

				remove_newlines(m_text);
			}
			else {
				if (m_text.size() < this->m_maxlen)
				{
					capture_input(m_text);
				}
			}
		}
	}

	virtual void render() override
	{
		//main
		d::antialias(false);
		d::rect1(rx, ry, w, h, mcolors::ctrlbck, 255, 1);

		//set up clip
		auto oldclip = d::get_clip();
		d::set_clip1(rx + m_textpad, ry, w - m_textpad * 2, h);

		//should flip render orientation?
		d::set_font(f::tahoma);
		auto dim = d::get_string_dim_a(m_text.c_str());
		bool fliprender = dim.w >= w - m_textpad;
		//set x position based on render
		float tx = fliprender ? (rx + w - m_textpad) : (rx + m_textpad);

		//string
		d::set_text_vertical_centering(true);
		d::set_text_outline(false);
		if(!m_text.size())
			d::string_a(rx + w / 2, ry + h / 2, DT_CENTER, color::white, 100, m_hint.c_str());
		else
		{
			if (m_hidetext)
			{
				std::string censored_text(m_text.size(), m.hide_char_replacement);
				d::string_a(tx, ry + h / 2, fliprender ? DT_RIGHT : DT_LEFT, color::white, 255, censored_text.c_str());
			}
			else
			{
				d::string_a(tx, ry + h / 2, fliprender ? DT_RIGHT : DT_LEFT, color::white, 255, m_text.c_str());
			}
		}

		//restore clip
		d::set_clip(oldclip);

		//internal animation
		d::rect1(rx, ry, w, h, mcolors::hover, m_animstate_alpha, 1);

		//outline
		d::rect1(rx, ry, w, h, m_inside || m.active_control == this ? mcolors::outlinelighter : mcolors::outline, 255);
	}

	void handle_callbacks() override
	{
		return;
	}

	virtual void handle_tooltips() override
	{
		return;
	}

	virtual int whoami() override
	{
		return controls::TextBox;
	}

	virtual void post_render() override {
		return;
	}

	virtual void post_think() override {
		return;
	}

	std::string get() {
		return m_text;
	}

	std::string m_hint;
	std::string m_text;
	int m_maxlen = 32;
	int m_textpad = 4;
	float m_lastdelete = 0.f;
	bool m_hidetext = 0;
};