#pragma once
#include "color.h"

class cbasecontrol;

struct {
	//main toggle
	bool opened = true;

	//input block
	bool block_input = false;

	//menu focus
	bool show_menu_focus = true;
	float menu_focus_alpha = 60.f;

	//animation speeds
	float aspeed = 0.005f;
	float aspeed1 = 0.001f;
	float aspeedtooltip = 0.01f;

	//character used for hiding the string in the input box
	char hide_char_replacement = '*';

	//milliseconds to wait before deleting a character
	float wait_amount_between_delete = 64.f;

	//max current control overcast alpha
	float maxalpha = 50.f;

	//max alpha for animations2
	float maxalpha1 = 150.f;

	//max alpha of gradient overlay
	float maxalpha_gradient = 80.f;

	//frame count
	int framecount = 0;

	//text padding
	float textpadx = 0.f;
	float textpady = 2.f;

	//tooltip padding
	float tooltippad = 2.f;

	//extra padding for tooltip box
	float tooltip_extra_x = 4.f;
	float tooltip_extra_y = 3.f;

	//main padding
	int padx = 10;
	int pady = 10;

	//how far is the next control being placed from the first
	int ctrlpad = 5;

	//member padding |<-->[x]<-->checkbox
	int mpad = 2;

	//active stuff
	cbasecontrol* active_control = nullptr;
}m;

enum controls
{
	Label,
	ListBox,
	Button,
	Checkbox,
	SliderFloat,
	GroupBox,
	Window,
	TextBox,
	ColorButton,
	TabControl,
	Tab,
	KeyBind,
	Picture,
	ComboBox
};

namespace mcolors
{
	//color_t bck = { 41, 43, 46 };
	color_t bck = { 60, 60, 60 };
	color_t ctrl = { 144, 1, 247 };
	color_t ctrlbck = { 50, 50, 50 };
	color_t outline = { 21,21,21 };
	color_t outlinelighter = { 15,15,15 };
	color_t hover = { 163, 163, 163 };

	//colors for gradient overlay
	color_t gradient_a = { 255,255,255 };
	color_t gradient_b = { 0,0,0 };

	//alpha values for the outlines
	namespace a
	{
		int outline_a = 130;
		int outline_a1 = 100;
	}
}