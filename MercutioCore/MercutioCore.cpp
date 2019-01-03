#include "stdafx.h"
#include <iostream>
#include "meTestEnv.h"
#include "meMemoryManager.h"
#include "meEntity.h"

class Sub : public Entity
{
public:
	std::string str = "I'm special";
	virtual Entity* New()
	{
		return new Sub();
	}
	virtual void Copy(Entity* source)
	{
		//std::cout << typeid(*this).name() << "\n";
		Sub* temp = dynamic_cast<Sub*>(source);
		*this = *temp;
		CopyChildren(source);
	}
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
	ent->Free();
	ent = (Sub*)memoryManager.Instantiate(&prototype);
	std::cout << ent->str << "\n";
	//std::cout << ent->str << "\n";
	entities.push_back(ent);
	std::cout << entities.size() << "\n";
	int i;
	std::cin >> i;
	return 0;
}

