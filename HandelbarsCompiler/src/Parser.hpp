#pragma once
#include "Utils.hpp"
#include <map>

namespace hbs
{
	class Parser
	{
	public:
		Parser(const std::string &path)
		{
			loadTemplates(path);
		
			for (auto& itr : files)
			{
				std::cout << itr.first << ":\n" << itr.second.hbs << "\nJson:\n" << itr.second.json << "\n\n\n";
			}
		}

	private:
		void loadTemplates(const std::string &path)
		{
			std::string Filepath = path;
			do
			{
				std::ifstream fin(Filepath);
				std::string fileStr((std::istreambuf_iterator<char>(fin)), std::istreambuf_iterator<char>());

				std::string hbs(parse_hbsToString(fileStr));
				nlohmann::json json = parse_hbsToJson(fileStr);

				files.insert(std::pair<std::string, FileData>(Filepath, { hbs, json }));
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

			/*for (int i = static_cast<int>(pathTree.size()) - 1; i >= 0; --i)
			{
				if (files[pathTree[i]].hbs.find("{{ include") != files[pathTree[i]].hbs.npos)
				{
					loadIncludeTree(Filepath);
				}
			}*/
		}

		void loadIncludeTree(const std::string &path)
		{
			
		}
		
	private:
		nlohmann::json globalJson;

		struct FileData
		{
			std::string hbs;
			nlohmann::json json;
		};

		std::map<std::string, FileData> files;
		std::vector<std::string> pathTree;
	};
}