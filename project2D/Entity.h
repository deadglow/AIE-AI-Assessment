#pragma once
#include "Transform.h"
#include <unordered_map>
#include <string>
#include <typeindex>

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

	int GetComponentCount();

	Entity* Clone();

	void Start();

	void Update();

	void OnCollision();

	std::string GetName();
	void SetName(std::string);

protected:
	std::string name;
	Transform* transform;
	std::unordered_map<std::type_index, Component*> components;
};