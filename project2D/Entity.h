#pragma once
#include "ComponentIncludes.h"
#include <unordered_map>
#include <string>
#include <typeindex>

class Game2D;

class Entity
{
public:
	//Pass gamedata so components have easier access to gamedata
	Entity(Game2D* gameData);
	~Entity();
	
	Game2D* GetGameData();

	//Returns a component of given type, otherwise returns null if not found
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

	//Adds a component of given type
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
	
	//Removes component of given type if found, otherwise throws an error
	template <class T>
	void RemoveComponent()
	{
		auto& result = components.find(typeid(T));

		if (result != components.end())
		{
			delete result->second;
			components.erase(result->first);
		}
		else
		{
			throw "Component not found.";
		}
	}


	Transform* GetTransform();

	size_t GetComponentCount();

	Entity* Clone();

	//This doesn't actually get called ever but i'm too scared to delete it
	void OnCreate();
	
	//Called when Destroy() is called before deleting
	void OnDestroy();

	//Called every frame, from the top of the tree downwards
	void Update();

	void OnCollision(Collision collision);

	//Create entity from mainScene
	Entity* CreateEntity(Transform* transform);
	Entity* CreateEntity();

	void Destroy();

	std::string GetName();
	void SetName(std::string);

	int GetLayer();
	void SetLayer(int newLayer);

protected:
	Game2D* gameData;
	std::string name;
	//Was supposed to be used in colliders but no more
	int layer;
	Transform* transform;
	//umap that uses type as a key, so components can be added as a generic
	std::unordered_map<std::type_index, Component*> components;
};