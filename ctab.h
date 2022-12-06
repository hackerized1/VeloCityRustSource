#pragma once
#include "cbasecontrol.h"
#include "drawing.h"
#include "input.h"
#include <vector>

using namespace input;

class ctab : public cbasecontrol
{
public:
	ctab(const std::wstring& name)
	{
		m_name = name;
	}

	virtual void think() override
	{
		for (const auto& ctrl : m_controls)
		{
			ctrl->think();
			ctrl->post_think();
		}
	}

	virtual void render() override
	{
		for (const auto& ctrl : m_controls)
		{
			ctrl->render();
			ctrl->post_render();
		}
	}

	void handle_callbacks() override
	{
		return;
	}

	virtual void handle_animations() override
	{
		return;
	}

	virtual void handle_tooltips() override
	{
		return;
	}

	virtual int whoami() override
	{
		return controls::Tab;
	}

	virtual void post_render() override {
		return;
	}

	virtual void post_think() override {
		return;
	}

	std::wstring m_name;
	std::vector<cbasecontrol*> m_controls;
};