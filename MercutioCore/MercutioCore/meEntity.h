#pragma once
#include <vector>
#include <string>
#include <map>
#include "meTransform.h"
#include "meEntityUtils.h"
class Entity
{
	//private fields
	Entity* parent;
	std::vector<Entity*> children;
	std::map<std::string, uint32_t> registeredMessages;
	Transform transform;
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
	void Entity::RemoveChild(Entity* child);
	void Entity::RemoveChildAt(uint32_t index);
	void SendMessage(std::string message, void* messageData = nullptr);
	void Instantiate(Entity* ent);
	
	//virtual methods
	virtual void ProcessMessage(uint32_t index, void* messageData = nullptr);

	//constructor / destructor
	Entity();
	virtual ~Entity();
};

