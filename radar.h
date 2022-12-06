#pragma once
#include "window.h"
#include "player.h"
#include "item.h"
#include "defs.h"
#include "config.h"
#include "globals.h"

namespace radar
{
	math::vector2 pos;
	math::vector2 size;

	void scale()
	{

		pos = { w::border,
				w::border };


		size = { w::width - w::border * 2.f,
				 w::height - w::border * 2.f };

		//menu::x = window::size.x / 2.f - menu::w / 2.f;
		//menu::y = window::size.y / 2.f - menu::tabsizes[0] / 2.f;
	}

	math::vector3 rotatePoint(const math::vector3& rawpoint, float angle)
	{
		float cosa = cos(angle);
		float sina = sin(angle);

		float outx = (rawpoint.x * cosa) + (-rawpoint.y * sina);
		float outy = (rawpoint.x * sina) + (rawpoint.y * cosa);

		return { -outx , outy,0 };
	}

	void render(const structures::BasePlayer_t& entity)
	{
		if (!globals::objects::localPlayer.input) return;

		bool playerSleeping = player::checkFlag(entity, FL_SLEEPING);
		if (playerSleeping && !config::radar::showSleeper) return;


		//body angles y
		float yaw = driver::rpm<float>(globals::objects::localPlayer.input + 0x3C + 4);
		yaw += 180.f;
		yaw = (yaw * (float)PI / 180.0f);

		math::vector3 lpos = player::getBasePosition(globals::objects::localPlayer);
		math::vector3 epos = player::getBasePosition(entity);

		math::vector3 t = epos - lpos;
		t *= config::radar::zoom;

		math::vector3 diff = { t.x,t.z,t.y };
		math::vector3 out = rotatePoint(diff, yaw);

		//add middle
		float rmx = pos.x + size.x / 2.f;
		float rmy = pos.y + size.y / 2.f;

		out.x += rmx;
		out.y += rmy;

		//clamp
		out.x = std::clamp(out.x, pos.x, pos.y + size.x);
		out.y = std::clamp(out.y, pos.y, pos.y + size.y);


		DWORD color;
		if (playerSleeping)
		{
			color = color2dx({ 110, 110, 110 }, 255);
		}
		else if (entity._maxHealth > 100.f)
		{
			if (!config::radar::showNPC) return;
			color = color2dx({ 64, 172, 189 }, 255);
		}
		else if (player::isInSameTeam(globals::objects::localPlayer, entity))
		{
			color = color2dx({ 168, 235, 52 }, 255);
		}
		else
		{
			color = color2dx({ 176, 0, 0 }, 255);
		}

		//drawing part
		float dotsize = config::radar::dotSize;


		//save old rect
		RECT old = d::get_clip();

		//our custom clip rectangle
		RECT newrect = { pos.x + 1,pos.y + 1,pos.x + size.x - 1,pos.y + size.y - 1 };

		//override
		d::set_clip(newrect);

		d::antialias(false);
		
		//outline
		//d::rect1(out.x - dotsize, out.y - dotsize, dotsize * 2.f, dotsize * 2.f, { 0,0,0 }, 255, 1);
		//main
		//d::rect1(out.x - dotsize, out.y - dotsize, dotsize * 2.f, dotsize * 2.f, dx2color(color), 255, 1);

		d::antialias(true);

		d::circle(out.x, out.y, dotsize + 1, 30, color::black, 255, 1);
		d::circle(out.x, out.y, dotsize, 30, dx2color(color), 255, 1);

		//controls the arrows
		constexpr float decx = 0.6;
		constexpr float decy = 0.8;
		constexpr float decpos = 0.5;

		float medium = 1.5f;
		if (lpos.y > epos.y + medium)
		{
			//down
			//v
			d::line(out.x - dotsize * decx, out.y - dotsize * decpos, out.x, out.y + dotsize * decy, color::white, 255, 2, 1);
			d::line(out.x + dotsize * decx, out.y - dotsize * decpos, out.x, out.y + dotsize * decy, color::white, 255, 2, 1);
			//connector
			d::line(out.x - dotsize * decx, out.y - dotsize * decpos, out.x, out.y - dotsize * 0.2, color::white, 255, 2, 1);
			d::line(out.x + dotsize * decx, out.y - dotsize * decpos, out.x, out.y - dotsize * 0.2, color::white, 255, 2, 1);
		}
		else if (lpos.y < epos.y - medium)
		{
			//up
			//V
			d::line(out.x - dotsize * decx, out.y + dotsize * decpos, out.x, out.y - dotsize * decy, color::white, 255, 2, 1);
			d::line(out.x + dotsize * decx, out.y + dotsize * decpos, out.x, out.y - dotsize * decy, color::white, 255, 2, 1);
			//connector
			d::line(out.x - dotsize * decx, out.y + dotsize * decpos, out.x, out.y + dotsize * 0.2, color::white, 255, 2, 1);
			d::line(out.x + dotsize * decx, out.y + dotsize * decpos, out.x, out.y + dotsize * 0.2, color::white, 255, 2, 1);
		}
		else {
			//normal
			d::circle(out.x, out.y, dotsize * 0.4, 30, color::white, 255, 0);
		}

		d::antialias(false);

		//spahgetegaewghuhyy coden
		//radar icon
		//static auto radar_icon = new ctexture(d::m_pdevice, 200, 200, radaricon, sizeof radaricon);
		//D3DXVECTOR2 cen(100, 114);
		//D3DXVECTOR2 pos(out.x, out.y);
		//float scalef = ui::sliderf_radar_dot_size->get();
		//scalef /= 10;
		//D3DXVECTOR2 scale(scalef, scalef);
		//D3DXMATRIX matrix;
		//
		////body angles y
		//float rot = driver::rpm<float>(entity.playerModel + 0x20 + 4);
		//rot += 180.f;
		//rot = (rot * (float)PI / 180.0f);
		//rot -= yaw;
		//
		//D3DXMatrixTransformation2D(&matrix, NULL, NULL, &scale, NULL, rot, &pos);
		//radar_icon->sprite->SetTransform(&matrix);
		//
		//radar_icon->center_shift(100, 114);
		//radar_icon->color_shift(color);
		//radar_icon->render(0, 0, 200, 200);

		//draw anything here that needs clipping
		if (!playerSleeping)
		{
			d::set_font(f::tahoma);
			d::set_text_outline(false);
			d::set_text_vertical_centering(false);

			d::string_a(out.x, out.y + dotsize + 1, DT_CENTER, { 255,255,255 }, 255, xorstr("%ls"), player::getName(entity).buffer);

			d::string_a(out.x, out.y + dotsize + 1 + 10, DT_CENTER, { 255,255,255 }, 255, xorstr("%.0f"), entity._health);

			d::string_a(out.x, out.y + dotsize + 1 + 20, DT_CENTER, { 255,255,255 },255, xorstr("%ls"), item::getName(player::getActiveItem(entity)).buffer);
		}
		else
		{
			d::string_a(out.x, out.y + dotsize + 1, DT_CENTER, { 255,255,255 }, 255, xorstr("%ls"), player::getName(entity).buffer);

			d::string_a(out.x, out.y + dotsize + 1 + 10, DT_CENTER, { 255,255,255 }, 255, xorstr("(Sleeping)"));
		}

		//restore old rect
		d::set_clip(old);
	}

	void renderBackground(bool fill = true, render_callback_t callback = nullptr)
	{
		scale();

		d::antialias(false);
		//whole fill
		d::rect1(0, 0, w::width, w::height, { 40, 40, 40 }, 255, 1);

		//inside fill
		if (fill)
		{
			d::rect1(pos.x, pos.y, size.x, size.y, { 42,47,56 }, 255, 1);
		}
		else {
			auto rect = d::get_clip();
			d::set_clip1(pos.x, pos.y, size.x, size.y);
			callback();
			d::set_clip(rect);
		}

		//lines
		if (fill)
		{
			d::line(pos.x, pos.y, pos.x + size.x / 2.f, pos.y + size.y / 2.f, { 0,0,0 }, 255, 1, 1);
			d::line(pos.x + size.x, pos.y, pos.x + size.x / 2.f, pos.y + size.y / 2.f, { 0,0,0 }, 255, 1, 1);
			d::line(pos.x + size.x / 2.f, pos.y, pos.x + size.x / 2.f, pos.y + size.y, { 0,0,0 }, 255, 1, 1);
			d::line(pos.x, pos.y + size.y / 2.f, pos.x + size.x, pos.y + size.y / 2.f, { 0,0,0 }, 255, 1, 1);
		}

		//inside outline
		d::rect1(pos.x, pos.y, size.x, size.y, mcolors::ctrl, 180);
		//outline
		d::rect1(0, 0, w::width, w::height, mcolors::ctrl, 180);
	}
}
