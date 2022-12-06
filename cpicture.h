#pragma once
#include "cbasecontrol.h"
#include "drawing.h"

class cpicture : public cbasecontrol
{
public:
public:
	cpicture()
	{
	}

	virtual void think() override
	{
		return;
	}

	virtual void render() override
	{
		if (m_texture != nullptr)
		{
			m_texture->render();
		}
		else {
			d::antialias(false);
			d::rect1(this->rx, this->ry, this->w, this->h, color::red, 255);
			d::line(rx, ry, rx + w, ry + h, color::red, 255, 1, 1);
			d::line(rx + w, ry, rx, ry + h, color::red, 255, 1, 1);
		}
	}

	void handle_callbacks() override
	{
		return;
	}

	virtual int whoami() override
	{
		return controls::Picture;
	}

	virtual void post_render() override {
		return;
	}

	virtual void post_think() override {
		return;
	}

	ctexture* m_texture = nullptr;
};