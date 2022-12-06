#pragma once

#include <d3d9types.h>

struct color_t
{
	float r, g, b;
};

struct gradient_t
{
	color_t top;
	color_t bottom;
};

constexpr D3DCOLOR color2dx(const color_t& color, int alpha) { return D3DCOLOR_ARGB(alpha, (int)color.r, (int)color.g, (int)color.b); };

inline color_t dx2color(D3DCOLOR dxcolor)
{
	return { (float)((dxcolor >> 16) & 0xff) ,(float)((dxcolor >> 8) & 0xff),(float)(dxcolor & 0xff) };
}

namespace color
{
	color_t white = { 255,255,255 };
	color_t black = { 0,0,0 };
	color_t red = { 255,0,0 };
	color_t green = { 0,255,0 };
	color_t blue = { 0,0,255 };
	color_t yellow = { 255, 255,0 };
	color_t orange = { 255,128,0 };
	color_t lime = { 191,255,0 };
	color_t light_blue = { 0,255,255 };
	color_t sky_blue = { 0,191,255 };
	color_t purple = { 128,0,255 };
	color_t pink = { 255,0,191 };
	color_t gray = { 138, 138, 138 };

	//namespace menu
	//{
	//	color_t background = { 42,47,56 };
	//	color_t primary = { 247,181,56 };
	//	color_t secondary = { 219,124,38 };
	//}
}

//depricated REPLACE
#define olive(alpha)         D3DCOLOR_ARGB(alpha, 98, 140, 13)
#define black_1(alpha)       D3DCOLOR_ARGB(alpha, 1, 1, 1)
#define white(alpha)         D3DCOLOR_ARGB(alpha, 255, 255, 255)
#define black(alpha)         D3DCOLOR_ARGB(alpha, 0, 0, 0)
#define red(alpha)           D3DCOLOR_ARGB(alpha, 255, 0, 0)
#define green(alpha)         D3DCOLOR_ARGB(alpha, 0, 128, 0)
#define lawngreen(alpha)     D3DCOLOR_ARGB(alpha, 124, 252, 0)
#define blue(alpha)          D3DCOLOR_ARGB(alpha, 0, 200, 255)
#define deepskyblue(alpha)   D3DCOLOR_ARGB(alpha, 0, 191, 255)
#define skyblue(alpha)       D3DCOLOR_ARGB(alpha, 0, 122, 204)
#define yellow(alpha)        D3DCOLOR_ARGB(alpha, 255, 255, 0)
#define orange(alpha)        D3DCOLOR_ARGB(alpha, 255, 165, 0)
#define darkorange(alpha)    D3DCOLOR_ARGB(alpha, 255, 140, 0)
#define purple(alpha)        D3DCOLOR_ARGB(alpha, 125, 0, 255)
#define cyan(alpha)          D3DCOLOR_ARGB(alpha, 0, 255, 255)
#define pink(alpha)          D3DCOLOR_ARGB(alpha, 255, 20, 147)
#define gray(alpha)          D3DCOLOR_ARGB(alpha, 128, 128, 128)
#define darkgray(alpha)      D3DCOLOR_ARGB(alpha, 73, 73, 73)
#define darkergray(alpha)    D3DCOLOR_ARGB(alpha, 31, 31, 31)
#define brown(alpha)		 D3DCOLOR_ARGB(alpha, 255, 249, 220)
#define lightgrey(alpha)     D3DCOLOR_RGBA(169, 169, 169, alpha)