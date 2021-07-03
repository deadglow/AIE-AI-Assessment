#include "Component.h"
#include "Matrix3.h"
#include <vector>

using namespace MathMan;

class Transform : Component
{
public:
	using Component::Component;

	Transform* GetParent();
	void SetParent(Transform* transform);

	Transform* GetChild(int index);
	void _AddChild(Transform* child);
	void _RemoveChild(Transform* child);

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

	void Initialise()
	{
		
	}
protected:
	Transform* parent = nullptr;
	std::vector<Transform*> children;

	Matrix3 localTransform = Matrix3::Identity();
	Matrix3 globalTransform = Matrix3::Identity();
	Vector2 up;
	Vector2 right;
};