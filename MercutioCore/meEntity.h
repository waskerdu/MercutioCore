#pragma once
#include <vector>
#include <string>
#include <map>
#include "meTransform.h"
#include "meMemoryManager.h"

class Entity
{
	//private fields
	Entity* parent;
	
	std::map<std::string, uint32_t> registeredMessages;
	Transform transform;
	bool isFreed = false;
	bool isAwake = true;
	bool lastWakeState = false; // whether the entity was awake last frame
	bool privateAwakeState = true; //used to 
	friend class MemoryManager;
	MemoryManager* memoryManager;
public:
	//public fields (minimize/eliminate)
	std::vector<Entity*> children; //should be private but templates in mem manager get grumpy
private:
	//private methods
	void SetAwakeInternal(bool wake);

protected:
	//protected methods
	void AddMessage(std::string message, uint32_t index);
	void Instantiate(Entity* ent); //asks the memory manager for a new instance of a given entity

public:
	//public methods
	void SetParent(Entity* newParent);
	void AddChild(Entity* newChild);
	void RemoveChild(Entity* child);
	void RemoveChildAt(uint32_t index);
	void SendMessage(std::string message, void* messageData = nullptr); //other objects call this method to send messages to this entity
	void SetAwake(bool awake);
	bool IsAwake();
	
	//virtual methods
	virtual void Awake() {} // runs each time the entity is "woken up"
	virtual void Update() {} // is called every frame by the engine
	virtual void FixedUpdate() {} //is called by the engine at set intervals
	virtual void ProcessMessage(uint32_t index, void* messageData = nullptr) {} // interprets messages other objects send to this one

	//constructor / destructor
	Entity();
	virtual ~Entity();
};

