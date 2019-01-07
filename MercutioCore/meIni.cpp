#include "stdafx.h"
#include "meIni.h"
#include <fstream>
#include <iostream>
#include <algorithm>
#include <sstream>

void Ini::Load(std::string fname)
{
	//
	if (fname == "")
	{
		//
		if (filename == "")
		{
			std::cout << "filename required.\n";
			return;
		}
		fname = filename;
	}

	std::ifstream myFile;
	myFile.open(fname);
	if (myFile.is_open() == false)
	{
		std::cout << "file " << fname << " failed to open.\n";
		myFile.close();
		return;
	}

	Clear();

	filename = fname;

	std::string line;
	std::string tempLine;
	Section* section = nullptr;
	//section = new Section();
	//delete section;
	//section = nullptr;
	while (std::getline(myFile, line))
	{
		//read through file adding to data as we go
		//strip out whitespace until you find an equal sign
		size_t found = line.find("=");
		if (found == std::string::npos)
		{
			//kill the whitespace
			line.erase(std::remove_if(line.begin(), line.end(), isspace), line.end());
		}
		else
		{
			tempLine = line.substr(found);
			line.erase(std::remove_if(line.begin(), line.begin() + found, isspace), line.end());
			line += tempLine;
		}
		found = line.find("=");
		//if first character is a ";" ignore
		if (line[0] == ';') { continue; }
		//if first character is "[" and last character is "]" create a new section
		if (line[0] == '[' && line[line.length() - 1] == ']' && line.length() > 2)
		{
			// add new section
			section = new Section();
			data.insert(std::pair<std::string, Section*>(line.substr(1, line.length() - 2), section));
		}
		if (found == std::string::npos) { continue; } // ignore lines that are not sections or keys
		if (section == nullptr) { continue; }
		//find the first equals sign. everything before that is the key
		std::string key = line.substr(0, found);
		std::string value = line.substr(found + 1);
		section->insert(std::pair<std::string, std::string>(key, value));
	}

	myFile.close();
}

void Ini::Save(std::string fname)
{
	//
	if (fname == "")
	{
		//
		if (filename == "")
		{
			std::cout << "filename required.\n";
			return;
		}
		fname = filename;
	}

	std::ofstream myFile;
	myFile.open(fname);
	if (myFile.is_open() == false)
	{
		std::cout << "file " << fname << " failed to open.\n";
		myFile.close();
		return;
	}
	std::map<std::string, Section*>::iterator i;
	for (i = data.begin(); i != data.end(); i++)
	{
		//
		std::string section = "[" + i->first + "]\n";
		myFile.write( section.c_str(), section.length() );
		std::map<std::string, std::string>::iterator f;
		std::string key;
		std::string value;
		std::string line;
		for (f = i->second->begin(); f != i->second->end(); f++)
		{
			key = f->first;
			value = f->second;
			line = key + "=" + value + "\n";
			myFile.write(line.c_str(), line.length());
		}
		myFile.write(std::string("\n").c_str(), 1);
	}

	myFile.close();
}

std::string Ini::Get(std::string section, std::string key)
{
	if (data.count(section) == 0)
	{
		std::cout << "section " << section << " does not exist.\n";
		return "";
	}
	if (data.at(section)->count(key) == 0)
	{
		std::cout << "key " << key << " of section " << section << " does not exist.\n";
		return "";
	}
	return data.at(section)->at(key);
}
int64_t Ini::GetInt(std::string section, std::string key)
{
	try
	{
		int64_t out = std::stoi(Get(section, key));
		return out;
	}
	catch(std::invalid_argument const &e)
	{
		(void)e;
		std::cout << "'" << Get(section, key) << "' cannot be read as an integer\n";
		return 0;
	}
}
double Ini::GetDouble(std::string section, std::string key)
{
	try
	{
		double out = std::stod(Get(section, key));
		return out;
	}
	catch (std::invalid_argument const &e)
	{
		(void)e;
		std::cout << "'" << Get(section, key) << "' cannot be read as an double\n";
		return 0.0;
	}
}
void Ini::Set(std::string section, std::string key, std::string value)
{
	//
	Section* sectionPtr = nullptr;
	if (data.count(section) == 0)
	{
		sectionPtr = new Section();
		data.insert(std::pair<std::string, Section*>(section, sectionPtr));
		sectionPtr->insert(std::pair<std::string, std::string>(key, value));
		return;
	}
	if (data.at(section)->count(key) == 0)
	{
		data.at(section)->insert(std::pair<std::string, std::string>(key, value));
		return;
	}
	data.at(section)->at(key) = value;
}
void Ini::Set(std::string section, std::string key, int64_t value)
{
	Set(section, key, std::to_string(value));
}
void Ini::Set(std::string section, std::string key, double value)
{
	Set(section, key, std::to_string(value));
}

void Ini::Clear()
{
	std::map<std::string, Section*>::iterator i;
	for (i = data.begin(); i != data.end(); i++)
	{
		delete i->second;
	}
}

Ini::Section* Ini::GetSection(std::string section)
{
	return data[section];
}

Ini::Ini()
{
}


Ini::~Ini()
{
	Clear();
}
