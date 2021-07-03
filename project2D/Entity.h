#pragma once
#include "Component.h"
#include "Transform.h"
#include <unordered_map>
#include <string>
#include <typeindex>

class Component;
class Transform;

class Entity
{
public:
	Entity();
	~Entity();

	template <class T>
	T* GetComponent()
	{
		auto result = components.find(typeid(T));
		if (result != components.end())
		{
			return (T*)(result->second);
		}
		else
		{
			return nullptr;
		}
	}

	template <class T>
	T* AddComponent()
	{
		auto result = components.find(typeid(T));

		if (result != components.end())
		{
			throw "bruh this already exists";
		}
		else
		{
			T* newComponent = new T(this);
			components.insert({ typeid(T), newComponent });
			return newComponent;
		}
	}
	
	template <class T>
	void RemoveComponent()
	{
		auto result = components.find(typeid(T));

		if (result != components.end())
		{
			delete result->second;
			components.erase(result);
		}
		throw "Component not found.";
	}

	Transform* GetTransform();

	int GetComponentCount();

	Entity* Clone();

	void Start();

	void Update();

	void OnCollision();

	std::string GetName();
	void SetName(std::string);

protected:
	std::string name = "Entity";
	Transform* transform;
	std::unordered_map<std::type_index, Component*> components;
};