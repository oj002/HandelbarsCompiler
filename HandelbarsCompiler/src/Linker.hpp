#pragma once
#include "Parser.hpp"

namespace hbs
{
	class Linker
	{
	public:
		explicit Linker(Parser& p)
			: parser(p)
		{
			linkTemplates(parser.pathTree.size() - 1);
			linkIncludes(linkTemplatesStr);
		}

	private:
		std::string linkTemplatesStr;
		void linkTemplates(size_t index)
		{
			std::string &str(parser.files[parser.pathTree[index]].hbs);

			size_t lastmatchPos = 0;
			static const std::regex regex(R"(\{\{(\s?)content(\s?)\}\})", std::regex::optimize);
			std::smatch match;
			while (std::regex_search(str, match, regex))
			{
				linkTemplatesStr += match.prefix();
				lastmatchPos = match.position();
				str.erase(static_cast<size_t>(match.position()), match.str().size());
				linkTemplates(index - 1);
			}
			linkTemplatesStr += str.substr(lastmatchPos, str.size());
		}

		void linkIncludes(std::string& fileStr)
		{
			std::string file{ fileStr };
			static const std::regex regex(R"lit(\{\{(\s?)include(\s?)"(.+)"(\s?)\}\})lit", std::regex::optimize);
			std::smatch match;
			std::smatch matchIncludePath;
			while (std::regex_search(file, match, regex))
			{
				static const std::regex regexIncludePath("\"(.+)\"", std::regex::optimize);
				std::string regexResult{ match.str() };
				std::regex_search(regexResult, matchIncludePath, regexIncludePath);

				std::string includePath{ matchIncludePath.str().substr(1, matchIncludePath.str().size() - 2) };
				includePath += ".hbs";

				linkIncludesStr += match.prefix();
				file = match.suffix();
				linkIncludes(parser.files[includePath].hbs);
			}
			linkIncludesStr += file;
		}


	public:
		std::string linkIncludesStr;
		Parser &parser;
	};
} // namespace hbs