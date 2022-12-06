#pragma once

#include "color.h"
#include <cmath>
#include <algorithm>
#include "clock.h"

// lerps colors based on start and end
class ccolorlerp
{
public:
	ccolorlerp(color_t start_, color_t end_, float speed_, bool looping_ = false)
	{
		start = start_, end = end_, speed = speed_;
		current = start_;
		percentage = 0;
		looping = looping_;
		reversed = false;
		finished = false;
	}

	void run()
	{
		if (finished && !looping)
			return;

		if (reversed)
		{
			current.r = std::lerp(end.r, start.r, percentage / 100.f);
			current.g = std::lerp(end.g, start.g, percentage / 100.f);
			current.b = std::lerp(end.b, start.b, percentage / 100.f);
		}
		else {
			current.r = std::lerp(start.r, end.r, percentage / 100.f);
			current.g = std::lerp(start.g, end.g, percentage / 100.f);
			current.b = std::lerp(start.b, end.b, percentage / 100.f);
		}

		percentage += speed * c::deltatime;
		percentage = std::clamp(percentage, 0.f, 100.f);

		if (percentage >= 100)
		{
			finished = true;
			if (looping)
			{
				reversed = !reversed;
				if (reversed)
				{
					current = end;
					percentage -= 100.f;
				}
			}
		}

		if (looping && finished)
			reset();
	}

	void reset()
	{
		if (!looping) current = start;
		percentage = 0;
		finished = false;
	}

	color_t& get_inc()
	{
		run();
		return current;
	}

	color_t& get() { return current; }

public:
	color_t start, end, current;
	float percentage, speed;

private:
	bool finished;
	bool looping;
	bool reversed;
};