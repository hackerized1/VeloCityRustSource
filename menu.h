#pragma once
#include <algorithm>
#include <vector>
#include "input.h"
#include "cwindow.h"

namespace menu_windows
{
	std::vector<cwindow*> windowlist;
}

namespace menu_windows
{
	using namespace input;

	cwindow* get_active_window() {
		return windowlist.back();
	}

	bool swap_window(cwindow* other) {
		if (other->m_hidden) return false;
		if (other->m_disabled) return false;

		for (int i = 0; i < windowlist.size(); i++) {

			if (windowlist[i] == other) {
				std::rotate(windowlist.begin() + i, windowlist.begin() + i + 1, windowlist.end());
				return true;
			}
		}

		return false;
	}

	//returns true if other window took focus
	bool handle_focusing()
	{
		if (key_pressed(VK_LBUTTON))
		{
			//if there is only one window no point in running this
			if (windowlist.size() > 1)
			{
				auto current_window = get_active_window();

				//if we clicked outside the main window start checking if the click landed based on the z allignment (thats why the loop is backwards)
				if (!mouse_in_region(current_window->ox, current_window->oy, current_window->w, current_window->h))
				{
					//backwards loop minus current active window
					for (int i = windowlist.size() - 2; i >= 0; i--)
					{ 
						auto win = windowlist[i];
						if (win->m_hidden) continue;

						if (mouse_in_region(win->ox, win->oy, win->w, win->h))
						{
							std::rotate(windowlist.begin() + i, windowlist.begin() + i + 1, windowlist.end());
							return true;
						}
					}
				}
			}
		}

		return false;
	}

	void remove_top()
	{
		if (windowlist.size())
		{
			windowlist.pop_back();
		}
	}

	void render()
	{
		if (windowlist.size())
		{
#ifdef NDEBUG
			if (input::key_pressed(VK_INSERT)) m.opened = !m.opened;
#endif

			if (m.opened == false)
				return;

			auto topwindow = get_active_window();

			//first we handle what window we will focus
			handle_focusing();

			//let the top window process input
			if (!m.block_input)
			{
				topwindow->think();
				topwindow->post_think();
			}

			//render windows
			for (const auto& window : windowlist)
			{
				//update window render position vars
				window->setup_draw();

				//render the window
				window->render();
				window->post_render();

				if (!window->m_hidden)
				{
					for (const auto& ctrl : window->m_controls)
					{
						if (ctrl->m_parent)
						{
							//sets anchor point to the window's 
							ctrl->ax = ctrl->m_parent->ox;
							ctrl->ay = ctrl->m_parent->oy;
						}

						//update control position
						ctrl->setup_draw();

						//render the control
						ctrl->render();
						ctrl->post_render();
					}
				}

				for (const auto& c : topwindow->m_controls)
				{
					c->handle_animations();
				}

				//draws boxes on all except top window
				if (m.show_menu_focus)
				{
					if (window != topwindow && !window->m_hidden)
					{
						d::rect1(window->ox, window->oy, window->w, window->h, color::black, m.menu_focus_alpha, 1);
					}
				}
			}

			//only handle tooltips for the top window

			//tooltip handler
			for (const auto& c : topwindow->m_controls)
			{
				c->handle_tooltips();
			}
		}
	}
}