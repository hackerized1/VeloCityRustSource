#pragma once
#include <Windows.h>

namespace input
{
	struct vector2
	{
		float x, y;
	};

	vector2 mouse_pos;

	bool key_state[256] = { 0 };
	bool prev_key_state[256] = { 0 };

	bool key_pressed(int key) noexcept
	{
		return key_state[key] == true && prev_key_state[key] == false;
	}

	bool key_down(int key) noexcept
	{
		return key_state[key] == true;
	}

	bool key_released(int key) noexcept
	{
		return key_state[key] == false && prev_key_state[key] == true;
	}

	bool mouse_in_region(const int x, const int y, const int w, const int h)
	{
		return mouse_pos.x >= x && mouse_pos.y >= y && mouse_pos.x <= w + x && mouse_pos.y <= h + y;
	}

	void poll_input(vector2 mouse_position)
	{
		for (int i = 0; i < 256; i++) {
			prev_key_state[i] = key_state[i];
			key_state[i] = GetAsyncKeyState(i);
		}

		mouse_pos = mouse_position;
	}
}