#include "stdafx.h"
#include "meEntity.h"
#include <iostream>

void Entity::SetAwakeInternal(bool awake)
{
	isAwake = awake;
	for (size_t i = 0; i < children.size(); i++)
	{
		children[i]->SetAwakeInternal(awake);
	}
}

void Entity::AddMessage(std::string message, uint32_t index)
{
	registeredMessages.emplace(message, index);
}

Entity* Entity::Instantiate(Entity* ent)
{
	return (Entity*)memoryManager->Instantiate(ent);
}

void Entity::SetParent(Entity* newParent)
{
	if (parent != nullptr)
	{
		parent->RemoveChild(this);
	}
	parent = newParent;
}

void Entity::AddChild(Entity* newChild)
{
	children.push_back(newChild);
	newChild->parent = this;
}

Entity* Entity::GetChildAt(uint32_t index)
{
	return children[index];
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

void Entity::RemoveChildAt(uint32_t index)
{
	children[index]->parent = nullptr;
	children.erase(children.begin() + index);
}

void Entity::RemoveAllChildren()
{
	for (size_t i = 0; i < children.size(); i++)
	{
		children[i]->SetParent();
	}
	children.clear();
}

size_t Entity::ChildCount()
{
	return children.size();
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

void Entity::SetAwake(bool awake)
{
	privateAwakeState = awake;
	SetAwakeInternal(awake);
}

bool Entity::IsAwake()
{
	return isAwake && lastWakeState && privateAwakeState;
}

void Entity::Free()
{
	isFreed = true;
}

Entity::Entity()
{
}


Entity::~Entity()
{
}
