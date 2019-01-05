#include "stdafx.h"
#include <iostream>
#include "meTestEnv.h"
#include "meMemoryManager.h"
#include "meEntity.h"
#include "meEngine.h"
#include "meIni.h"

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
	/*TestEnv test;

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
	
	Ini ini;
	ini.Load("gamefiles/test.ini");
	ini.Set("test2", "plort", "fred");
	ini.Set("test2", "tom", "paris");
	ini.Set("test3", "tom", "paris");
	std::cout << ini.Get("test3", "tom") << "\n";
	std::cout << ini.GetInt("test3", "tom") << "\n";
	std::cout << ini.GetDouble("test", "sum") << "\n";
	ini.Save("gamefiles/testout.ini");*/
	// cd documents/"visual studio 2015"/projects/mercutiocore
	Engine engine;
	engine.Launch();

	//int i = 0;
	//std::cin >> i;
	return 0;
}

