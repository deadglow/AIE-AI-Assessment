#include "Transform.h"

Transform* Transform::GetParent()
{
	return parent;
}

void Transform::SetParent(Transform* transform)
{
	parent = transform;
}

Matrix3 Transform::GetLocalTransform()
{
	return localTransform;
}

Matrix3 Transform::GetGlobalTransform()
{
	return globalTransform;
}

Vector2 Transform::GetLocalPosition()
{
	return Vector2(localTransform.m[6], localTransform.m[7]);
}

void Transform::SetLocalPosition(Vector2 pos)
{
	localTransform.m[6] = pos.x;
	localTransform.m[7] = pos.y;
}

Vector2 Transform::GetWorldPosition()
{
	return Vector2(globalTransform.m[6], globalTransform.m[7]);
}

void Transform::SetGlobalPosition(Vector2 pos)
{
	//Implement dis
}

void Transform::Translate(Vector2 delta, bool moveLocal = true)
{
	if (moveLocal)
	{
		localTransform.m[6] += delta.x;
		localTransform.m[7] += delta.y;
	}
	else
	{
		Matrix3 translationMatrix = Matrix3::Identity();
		translationMatrix.m[6] = delta.x;
		translationMatrix.m[7] = delta.y;
		translationMatrix = translationMatrix * parent->globalTransform.Inverse();
	}
}

void Transform::UpdateGlobalMatrix()
{
	globalTransform = parent->GetGlobalTransform() * localTransform;


}