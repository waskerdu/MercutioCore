#include "stdafx.h"
#include "meMemoryManager.h"
#include <typeinfo>
#include <iostream>
#include "meEntity.h"

MemoryManager::MemoryManager()
{
}


MemoryManager::~MemoryManager()
{
	std::map<std::string, std::vector<void*>*>::iterator it;

	for (it = pools.begin(); it != pools.end(); it++)
	{
		for (size_t i = 0; i < it->second->size(); i++)
		{
			delete(it->second->at(i));
		}
		delete it->second;
	}
}

void* MemoryManager::Instantiate(void* e)
{
	Entity* ent = (Entity*)e;
	std::string name = typeid(*ent).name();
	if (pools.count(name) == 0)
	{
		//create a new pool
		pools[name] = new std::vector<void*>();
	}
	//get a free object if avalible. if not 
	//allocate a new object in the pool, copy e to it, and return it
	Entity* tempEnt;
	for (size_t i = 0; i < pools[name]->size(); i++)
	{
		tempEnt = (Entity*)pools[name]->at(i);
		if (tempEnt->isFreed)
		{
			//copy into tempEnt and return it
			tempEnt->isFreed = false;
			tempEnt->Copy(ent);
			initQueue.push_back(tempEnt);
			//std::cout << "reused\n";
			return tempEnt;
		}
	}
	tempEnt = ent->New();
	tempEnt->memoryManager = this;
	pools[name]->push_back(tempEnt);
	initQueue.push_back(tempEnt);
	std::cout << "created new " << name << "\n";
	return tempEnt;
}

void MemoryManager::AllocatePool(void* e, size_t num)
{
	//Should size the pool according to num. Will probably need to be done with calloc
	//Allocating a vector of entities isn't good enough because subclasses will be bigger
	//Need to find the biggest possible subclass, which will obviously vary by project
	Entity* ent = (Entity*)e;
	std::string name = typeid(*ent).name();
	if (pools.count(name) == 0)
	{
		//create a new pool
		pools[name] = new std::vector<void*>();
	}
}
