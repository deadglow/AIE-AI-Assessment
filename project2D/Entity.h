#pragma once
#include <map>
#include <string>
#include <typeindex>
#include <unordered_map>
#include "Component.h"
#include "Transform.h"

typedef std::unordered_map<std::type_index, Component*> CMap;

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
			return result->second;
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
			return;
		}
		else
		{
			components.insert(typeid(T), new T(this));
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
protected:
	std::string name;
	Transform* transform;
	CMap components;
};