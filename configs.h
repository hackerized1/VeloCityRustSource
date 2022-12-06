#pragma once
#include "editor.h"
#include <sstream>

std::vector<std::string> split(std::string s, std::string delimiter) {
	size_t pos_start = 0, pos_end, delim_len = delimiter.length();
	std::string token;
	std::vector<std::string> res;

	while ((pos_end = s.find(delimiter, pos_start)) != std::string::npos) {
		token = s.substr(pos_start, pos_end - pos_start);
		pos_start = pos_end + delim_len;
		res.push_back(token);
	}

	res.push_back(s.substr(pos_start));
	return res;
}

namespace config
{
	bool generate(std::vector<std::string>& out)
	{
		std::vector<std::string> config_lines;

		for (const auto& metadata : vars::callstack)
		{
			//filters out controls that dont need config support
			switch (metadata.control->whoami())
			{
			case controls::Button:
			case controls::GroupBox:
			case controls::Label:
			case controls::ListBox:
			case controls::Tab:
			case controls::TabControl:
			case controls::TextBox:
			case controls::Window:
			case controls::Picture:
			case controls::ComboBox:
				continue;
			default:
				break;
			}

			std::stringstream config_line;
			config_line << metadata.variable_name_hash << " ";

			switch (metadata.control->whoami())
			{
			case controls::Checkbox:
			{
				auto p = (ccheckbox*)metadata.control;
				config_line << p->get();
				break;
			}
			case controls::ColorButton:
			{
				auto p = (ccolorbutton*)metadata.control;
				config_line << color2dx(p->get(), 255);
				break;
			}
			case controls::KeyBind:
			{
				auto p = (ckeybind*)metadata.control;
				config_line << p->get();
				break;
			}
			case controls::SliderFloat:
			{
				auto p = (csliderfloat*)metadata.control;
				config_line << p->get();
				break;
			}
			default:
				break;
			}

			config_lines.push_back(config_line.str());
		}

		out = config_lines;
		return true;
	}

	bool load(const std::vector<std::string>& config_lines)
	{
		int good = 0;

		for (int i = 0; i < config_lines.size(); i++)
		{
			auto arguments = split(config_lines[i], " ");
			if (arguments.size() < 2)
				continue;

			//first argument is allways the hash
			auto hash = std::stoi(arguments[0], nullptr, 10);
			 
			//check if the hash exists in the metadata

			for (const auto& metadata : vars::callstack)
			{
				if (metadata.variable_name_hash == hash)
				{
					//found the control
					switch (metadata.control->whoami())
					{
					case controls::Checkbox:
					{
						auto p = (ccheckbox*)metadata.control;
						std::stringstream ss(arguments[1]);
						int newval = 0;
						ss >> newval;
						p->set(newval);
						break;
					}
					case controls::ColorButton:
					{
						auto p = (ccolorbutton*)metadata.control;
						std::stringstream ss(arguments[1]);
						DWORD newval = 0;
						ss >> newval;
						p->set(newval);
						break;
					}
					case controls::KeyBind:
					{
						auto p = (ckeybind*)metadata.control;
						std::stringstream ss(arguments[1]);
						int newval = 0;
						ss >> newval;
						p->set(newval);
						break;
					}
					case controls::SliderFloat:
					{
						auto p = (csliderfloat*)metadata.control;
						std::stringstream ss(arguments[1]);
						float newval = 0;
						ss >> newval;
						p->set(newval);
						break;
					}
					default:
						continue;
					}

					good++;
					continue;
				}
			}

		}

		return true;
	}
}
