#pragma once
#include <map>
#include <vector>
#include <string>

class MemoryManager
{
	std::map<std::string, std::vector<void*>*> pools;
	//temporary storage where entities are staged before they are grabbed by the engine
	std::vector<void*> initQueue;
public:
	MemoryManager();
	~MemoryManager();
	//General method to add new entities to the simulation.
	void* Instantiate(void* ent);
	//Method to preallocate memory in pools to avoid allocations during gameplay
	void AllocatePool(void* ent, size_t num); //not fully implimented yet
};

