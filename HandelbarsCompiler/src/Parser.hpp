#pragma once
#include "Utils.hpp"
#include <map>
#include <regex>

namespace hbs
{
	class Parser
	{
	public:
		explicit Parser(const std::string &path)
		{
			loadTemplates(path);
		
			/*for (auto& itr : files)
			{
				std::cout << itr.first << ":\n" << itr.second.hbs << "\nJson:\n" << itr.second.json << "\n\n\n";
			}

			std::cout << "global Json:\n" << globalJson << '\n';*/
		}

	private:
		void loadTemplates(const std::string &path)
		{
			bool once = true;
			std::string Filepath = path;
			do
			{
				std::ifstream fin(Filepath);
				std::string fileStr((std::istreambuf_iterator<char>(fin)), std::istreambuf_iterator<char>());

				std::string hbs(parse_hbsToString(fileStr));
				nlohmann::json json = parse_hbsToJson(fileStr);
				files.insert(std::pair<std::string, FileData>(Filepath, { hbs, json }));
				if (!json.empty())
				{
					if (once)
					{
						globalJson = json;
						once = false;
					}
					else
					{
						globalJson.insert(json.begin(), json.end());
					}
				}

				pathTree.push_back(Filepath);
				if (!json.empty())
				{
					Filepath = json.value("template", "") + ".hbs";
				}
				else
				{
					break;
				}

			} while (Filepath != ".hbs");

			for (int i = static_cast<int>(pathTree.size()) - 1; i >= 0; --i)
			{
				loadIncludeTree(pathTree[i]);
			}
			globalJson.erase("template");
		}

		void loadIncludeTree(const std::string &path)
		{
			std::string file{ files[path].hbs };
			std::regex regex("\\{\\{(\\s?)include(\\s?)\"(.+)\"(\\s?)\\}\\}");
			std::smatch match;
			while (std::regex_search(file, match, regex))
			{
				std::regex regexIncludePath("\"(.+)\"");
				std::smatch matchIncludePath;
				std::string regexResult{ match.str() };
				std::regex_search(regexResult, matchIncludePath, regexIncludePath);

				std::string includePath{ matchIncludePath.str().substr(1, matchIncludePath.str().size() - 2) };
				includePath += ".hbs";
				std::ifstream fin(includePath);
				std::string fileStr((std::istreambuf_iterator<char>(fin)), std::istreambuf_iterator<char>());

				std::string hbs(parse_hbsToString(fileStr));
				nlohmann::json json = parse_hbsToJson(fileStr);
				files.insert(std::pair<std::string, FileData>(includePath, { hbs, json }));
				globalJson.insert(json.begin(), json.end());

				file = match.suffix();
				loadIncludeTree(includePath);
			}
		}
		
	public:
		nlohmann::json globalJson;

		struct FileData
		{
			std::string hbs;
			nlohmann::json json;
		};

		std::map<std::string, FileData> files;
		std::vector<std::string> pathTree;
	};
} // namespace hbs