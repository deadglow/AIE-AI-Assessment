#pragma once
#include <vector>
#include "Matrix3.h"
#include "Component.h"

using namespace MathMan;

class Transform : public Component
{
public:
	using Component::Component;

	~Transform();

	Transform* CloneTo(Entity* ent) override;

	Transform* GetParent();
	void SetParent(Transform* transform);

	Transform* GetChild(int index);
	
	//Returns memory address of child transforms list
	std::vector<Transform*>* GetChildrenList();

	size_t GetChildCount();

	Matrix3 GetLocalTransform();
	Matrix3 GetGlobalTransform();

	Vector2 GetGlobalPosition();
	//DOESNT WORK YET
	void SetGlobalPosition(Vector2 pos);

	Vector2 GetLocalPosition();
	void SetLocalPosition(Vector2 pos);
	
	Vector2 GetUp();
	Vector2 GetRight();

	float GetGlobalRotation();
	void SetGlobalRotation(float radians);

	float GetLocalRotation();
	void SetLocalRotation(float radians);

	Vector2 GetLocalScale();
	void SetLocalScale(Vector2 scale);
	
	//Update this globalmatrix and recursively update child matrices
	void UpdateGlobalMatrix();

	void Translate(Vector2 delta, bool moveLocal = true);
	
	void Rotate(float radians);

	//Rotates object so up is pointing towards direction
	void SetUp(Vector2 direction);

	//Set the transform's up to face point
	void LookAt(Vector2 point);

protected:
	void _AddChild(Transform* child);
	void _RemoveChild(Transform* child);

	Transform* parent = nullptr;
	std::vector<Transform*> children;

	Matrix3 localTransform = Matrix3::Identity();
	Matrix3 globalTransform = Matrix3::Identity();
	Vector2 up;
	Vector2 right;
};