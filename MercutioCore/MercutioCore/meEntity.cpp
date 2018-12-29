#include "stdafx.h"
#include "meEntity.h"
#include <iostream>

void Entity::AddMessage(std::string message, uint32_t index)
{
	registeredMessages.emplace(message, index);
}

void Entity::SetParent(Entity* newParent)
{
	parent->RemoveChild(this);
	parent = newParent;
}

void Entity::AddChild(Entity* newChild)
{
	children.push_back(newChild);
	newChild->parent = this;
}

void Entity::RemoveChildAt(uint32_t index)
{
	children[index]->parent = nullptr;
	children.erase(children.begin() + index);
}

void Entity::RemoveChild(Entity* child)
{
	for (size_t i = 0; i < children.size(); i++)
	{
		if (child == children[i])
		{
			RemoveChildAt(i);
			return;
		}
	}
	std::cout << "Remove child failed: no such child exists.\n";
}

void Entity::SendMessage(std::string message, void* messageData)
{
	if (registeredMessages.count(message) == 0)
	{
		std::cout << "Message " << message << " was not recognized.\n";
		return;
	}
	ProcessMessage(registeredMessages.at(message), messageData);
}

void Entity::ProcessMessage(uint32_t index, void* messageData)
{
	//Intentionally left blank. Each class that inherits from Entity impliments their own.
}

void Entity::Instantiate(Entity* ent)
{
	//talk to memory manager
}

Entity::Entity()
{
}


Entity::~Entity()
{
}
