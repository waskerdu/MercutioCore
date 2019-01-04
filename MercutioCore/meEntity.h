#pragma once
#include <vector>
#include <string>
#include <map>
#include <iostream>
#include "meTransform.h"
#include "meMemoryManager.h"

class Entity
{
	//private fields
	Entity* parent;
	std::vector<Entity*> children;
	std::map<std::string, uint32_t> registeredMessages;
	Transform transform;
	bool isFreed = true;
	bool isAwake = true;
	bool lastWakeState = false; // whether the entity was awake last frame
	bool privateAwakeState = true; //used to 
	friend class Engine;
	//engine handles
	double deltaTime;
	friend class MemoryManager;
	MemoryManager* memoryManager;

public:
	//public fields (minimize/eliminate)
	
private:
	//private methods
	void SetAwakeInternal(bool wake);
	
	//private virtual methods

	//Every subclass of entity MUST impliment these methods or object slicing will occur.
	//It would be great if there was a less stupid way of doing this.
	virtual Entity* New();
	virtual void Copy(Entity* source);
	/*
	The following can be copied into the source straightforwardly.
	Replace {Type} with the name of the subclass.
	In the future this might be done with a fancy macro.
	In the future there will be a utility to ensure these methods are implimented.

	In header:
	virtual {Type}* New();
	virtual void Copy();

	In cpp:
	Entity* {Type}::New()
	{
		return new Entity();
	}
	void {Type}::Copy(Entity* source)
	{
		{Type}* temp = dynamic_cast<{Type}*>(source);
		*this = *source;
		CopyChildren(source);
	}
	*/

protected:
	//protected methods
	void AddMessage(std::string message, uint32_t index);
	void CopyChildren(Entity* source);

public:
	//public methods
	Entity* Instantiate(Entity* ent); //asks the memory manager for a new instance of a given entity
	void SetParent(Entity* newParent = nullptr);
	void AddChild(Entity* newChild);
	Entity* GetChildAt(uint32_t index);
	void RemoveChild(Entity* child);
	void RemoveChildAt(uint32_t index);
	void RemoveAllChildren();
	size_t ChildCount();
	void SendMessage(std::string message, void* messageData = nullptr); //other objects call this method to send messages to this entity
	void SetAwake(bool awake);
	bool IsAwake();
	void Free();
	
	//virtual methods
	virtual void Awake() {} // is called each time the entity is "woken up"
	virtual void Update() {} // is called every frame by the engine
	virtual void FixedUpdate() {} //is called by the engine at set intervals
	virtual void ProcessMessage(uint32_t index, void* messageData = nullptr) {} // interprets messages other objects send to this one

	//constructor / destructor
	Entity();
	virtual ~Entity();
};

