#pragma once
#include "ComponentIncludes.h"
#include <unordered_map>
#include <string>
#include <typeindex>

class Game2D;

class Entity
{
public:
	Entity(Game2D* gameData);
	~Entity();

	template <class T>
	T* GetComponent()
	{
		auto& result = components.find(typeid(T));
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

		if (components.find(typeid(T)) != components.end())
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
		auto& result = components.find(typeid(T));

		if (result != components.end())
		{
			delete result->second;
			components.erase(result->first);
		}
		throw "Component not found.";
	}

	Transform* GetTransform();

	Game2D* GetGameData();

	size_t GetComponentCount();

	Entity* Clone();

	void Start();

	void Update();

	void OnCollision(Collision collision);

	std::string GetName();
	void SetName(std::string);

	int GetLayer();
	void SetLayer(int newLayer);

	Entity* CreateEntity(Transform* transform);
	Entity* CreateEntity();

protected:
	Game2D* gameData;
	std::string name;
	int layer;
	Transform* transform;
	std::unordered_map<std::type_index, Component*> components;
};