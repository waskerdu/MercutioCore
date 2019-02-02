#pragma once
struct Asset
{
	bool isLoaded = false;
	bool isNeeded = false;
	std::vector<std::string> instructions;
	std::string name;
	void* data = nullptr;
	Asset() {}
	~Asset()
	{
		if (data != nullptr && isLoaded && instructions.size() != 0)
		{
			delete data;
		}
	}
};