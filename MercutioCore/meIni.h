#pragma once
#include <string>
#include <map>
#include <vector>

/*struct Section
{
	std::map<std::string, std::string> data;
	std::vector<std::string> orderedData;
	std::string operator[](std::string name)
	{
		return data[name];
	}
	std::vector<std::string>* GetOrderedData()
	{
		return nullptr;
	}
};/**/
class Ini
{
	typedef std::map<std::string, std::string> Section;

	std::string filename;
	std::map<std::string, Section*> data;

	void Clear();
public:
	void Load(std::string filename = "");
	void Save(std::string filename = "");
	std::string Get(std::string section, std::string key);
	int64_t GetInt(std::string section, std::string key);
	double GetDouble(std::string section, std::string key);
	void Set(std::string section, std::string key, std::string value);
	void Set(std::string section, std::string key, int64_t value);
	void Set(std::string section, std::string key, double value);
	Section* GetSection(std::string section);
	Ini();
	~Ini();
};

