#pragma once
#include "Linker.hpp"
namespace hbs
{
	class Compiler
	{
	public:
		explicit Compiler(Linker &l)
			: linker(l)
		{
			std::string str{ l.linkIncludesStr };
			static const std::regex regex(R"lit(\{\{([^\{\}]+)\}\})lit", std::regex::optimize);
			static std::smatch match;
			static std::smatch testMatch;
			while (std::regex_search(str, match, regex))
			{
				result += match.prefix();

				handleExpresions(match.str().c_str());

				str = match.suffix();
			}
			result += str;
			std::cout << result << '\n';
		}

	private:
		void handleExpresions(std::string str)
		{
			static const std::regex rawJsonRegex(R"lit(\{\{(\s?)page(\.(\w+))+(\s?)\}\})lit");
			static std::smatch rawJsonMatch;
			if (std::regex_match(str, rawJsonMatch, rawJsonRegex))
			{
				static const std::regex regex(R"((\.(\w+))+)");
				static std::smatch match;
				std::regex_search(str, match, regex);
				str = match.str();
				str.erase(0, 1);
				if (!linker.parser.globalJson.empty())
				{
					result += linker.parser.globalJson.value(str, "");
				}
			}

		}

	public:
		std::string result;
		Linker &linker;
	};
}  // namespace hbs