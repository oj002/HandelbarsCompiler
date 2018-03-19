#pragma once
#include "Utils.hpp"
#include <map>

namespace hbs
{
	class Parser
	{
	public:
		explicit Parser(const std::string &path)
		{
			loadTemplates(path);
		
			for (auto& itr : files)
			{
				std::cout << itr.first << ":\n" << itr.second.hbs << "\nJson:\n" << itr.second.json << "\n\n\n";
			}

			std::cout << "global Json:\n" << globalJson << '\n';
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
				if (files[pathTree[i]].hbs.find("{{ include") != files[pathTree[i]].hbs.npos)
				{
					loadIncludeTree(pathTree[i]);
				}
			}
			globalJson.erase("template");
		}

		void loadIncludeTree(const std::string &path)
		{
			FileData &file = files[path];
			size_t begPos = file.hbs.find("{{ include");
			size_t endPos = file.hbs.find("}}", begPos);
			while (begPos != file.hbs.npos && endPos != file.hbs.npos)
			{
				std::string subStr = file.hbs.substr(begPos + 12, endPos - (begPos + 12) - 2);
				removeFronBackWhitespaces(&subStr);
				subStr += ".hbs";

				std::ifstream fin(subStr);
				std::string fileStr((std::istreambuf_iterator<char>(fin)), std::istreambuf_iterator<char>());

				std::string hbs(parse_hbsToString(fileStr));
				nlohmann::json json = parse_hbsToJson(fileStr);
				files.insert(std::pair<std::string, FileData>(subStr, { hbs, json }));
				globalJson.insert(json.begin(), json.end());

				loadIncludeTree(subStr);

				file.hbs.erase(begPos, endPos - begPos + 2);
				begPos = file.hbs.find("{{ include");
				endPos = file.hbs.find("}}", begPos);
			}
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
} // namespace hbs