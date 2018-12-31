#pragma once

/*
Mercutio uses an object pooling memory management system and as such direct calls to new are not allowed.
This is the only place where entities can be allocated in memory.

Note: The Mercutio style guide does not normally allow templates. This is an exception.
The user should not be expected to duplicate the same CreateNew code in every class that inherits from entity.
*/

template <class T>
T* CreateNew(T* source)
{
	return new T(*source);
}
template <class T>
void Copy(T* source, T* dest)
{
	*dest = *source;
	dest->children.clear();
	for (unsigned int i = 0; i < source->children.size(); i++)
	{
		dest->children.push_back(Instantiate(source->children[i]));
		copy->children.back()->parent = dest;
	}
}
/*
Entity* Entity::Copy(Entity* dest)
{
	//copy creates a new object based on another object and returns it. sub classes will write their own version
	Entity* copy = new Entity(*this);
	if (dest == nullptr)
	{
		copy = new Entity(*this);
		DeepCopy(this, copy);
	}
	else
	{
		*copy = *dest;
	}
	return copy;
}

void Entity::DeepCopy(Entity* source, Entity* copy)
{
	//DeepCopy copies both the entity and its children.
	//Recursively walks the tree creating new objects as needed.
	copy->children.clear();
	copy->localTransform.position = source->localTransform.position;
	for (unsigned int i = 0; i < source->children.size(); i++)
	{
		copy->children.push_back(source->children[i]->Copy());
		//copy->children.push_back(Instantiate(source->children[i]));
		copy->initQueue.push_back(copy->children.back());
		copy->children.back()->parent = this;
	}
}/**/