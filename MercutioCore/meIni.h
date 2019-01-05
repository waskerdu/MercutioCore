#pragma once
#include <string>
#include <map>
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
	Ini();
	~Ini();
};

