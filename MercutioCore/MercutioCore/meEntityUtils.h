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
	return new T(source);
}