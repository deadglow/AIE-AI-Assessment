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

	Transform* GetParent();
	void SetParent(Transform* transform);

	Transform* GetChild(int index);
	
	void _AddChild(Transform* child);
	void _RemoveChild(Transform* child);
	std::vector<Transform*>* _GetChildrenList();

	int GetChildCount();

	Matrix3 GetLocalTransform();
	Matrix3 GetGlobalTransform();

	Vector2 GetGlobalPosition();
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
	
	void UpdateGlobalMatrix();

	void Translate(Vector2 delta, bool moveLocal = true);
	
	void Rotate(float radians);

protected:
	Transform* parent = nullptr;
	std::vector<Transform*> children;

	Matrix3 localTransform = Matrix3::Identity();
	Matrix3 globalTransform = Matrix3::Identity();
	Vector2 up;
	Vector2 right;
};