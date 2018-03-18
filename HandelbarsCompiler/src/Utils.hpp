#pragma once
#include <algorithm>
#include <cctype>
#include <fstream>
#include <iostream>

// https://github.com/nlohmann/json
#include "json.hpp"

namespace hbs
{
	nlohmann::json parse_hbsToJson(const std::string &str)
	{
		if (str.find("---") == str.npos)
		{
			return nlohmann::json();
		}
		std::istringstream stream(str.substr(0, str.find("---")));

		return nlohmann::json::parse(stream);
	}

	std::string parse_hbsToString(const std::string &str)
	{
		if (str.find("---") != std::string::npos)
		{
			return (str.substr(str.find("---") + 3, str.length()));
		}

		return str;
	}

	void removeFronBackWhitespaces(std::string * str)
	{
		str->erase(str->begin(), std::find_if(str->begin(), str->end(), [](int ch) { return std::isspace(ch) == 0; }));
		str->erase(std::find_if(str->rbegin(), str->rend(), [](int ch) { return std::isspace(ch) == 0; }).base(), str->end());
	}

} // namespace hbs