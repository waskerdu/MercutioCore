#include "stdafx.h"
#include <iostream>
#include "meTestEnv.h"
#include "meMemoryManager.h"
#include "meEntity.h"

class Sub : public Entity
{
public:
	std::string str = "I'm special";
};

int main()
{
	//TestEnv test;
	std::vector<Entity*> entities;
	MemoryManager memoryManager;
	Sub prototype;
	Sub* ent = (Sub*)memoryManager.Instantiate(&prototype);
	ent->str = "Me too!";
	entities.push_back(ent);
	/*ent->Free();
	ent = (Sub*)memoryManager.Instantiate(&prototype);
	std::cout << ent->str << "\n";
	entities.push_back(ent);/**/
	std::cout << entities.size() << "\n";
	for (size_t i = 0; i < entities.size(); i++)
	{
		delete entities[i];
	}
	int i;
	std::cin >> i;
	return 0;
}

