#pragma once
#include <vector>
#include <string>
#include <map>
class Entity
{
	//private fields
	Entity* parent;
	std::vector<Entity*> children;
	std::map<std::string, uint32_t> registeredMessages;
public:
	//public fields (minimize/eliminate)
	
private:
	//private methods

protected:
	//protected methods
	void AddMessage(std::string message, uint32_t index);

public:
	//public methods
	void SetParent(Entity* newParent);
	void AddChild(Entity* newChild);
	
	//virtual methods
	virtual void SendMessage(std::string message, void* messageData = nullptr);

	//constructor / destructor
	Entity();
	virtual ~Entity();
};

