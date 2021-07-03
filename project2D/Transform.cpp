#include "Transform.h"

Transform* Transform::GetParent()
{
	return parent;
}

void Transform::SetParent(Transform* transform)
{
	if (parent != nullptr)
		parent->_RemoveChild(this);

	parent = transform;

	parent->_AddChild(this);
}

Transform* Transform::GetChild(int index)
{
	return children[index];
}

void Transform::_AddChild(Transform* child)
{
	children.push_back(child);
}

std::vector<Transform*>* Transform::_GetChildrenList()
{
	return &children;
}

void Transform::_RemoveChild(Transform* child)
{
	auto iter = std::find(children.begin(), children.end(), child);

	if (iter == children.end())
		throw "Child not found!";

	children.erase(iter);
}

int Transform::GetChildCount()
{
	return children.size();
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

Vector2 Transform::GetUp()
{
	return up;
}

Vector2 Transform::GetRight()
{
	return right;
}

Vector2 Transform::GetGlobalPosition()
{
	return Vector2(globalTransform.m[6], globalTransform.m[7]);
}

void Transform::SetGlobalPosition(Vector2 pos)
{
	//Implement dis
}

float Transform::GetLocalRotation()
{
	return localTransform.GetRotation();
}

void Transform::SetLocalRotation(float radians)
{
	localTransform.SetRotation(radians);
}

float Transform::GetGlobalRotation()
{
	return globalTransform.GetRotation();
}

void Transform::SetGlobalRotation(float radians)
{
	Matrix3 rotMat = Matrix3::Identity();
	rotMat.SetRotateZ(radians);

	rotMat = rotMat * globalTransform.Inverse();
	rotMat.m[6] = 0;
	rotMat.m[7] = 0;

	rotMat.SetScaleX(1);
	rotMat.SetScaleY(1);

	localTransform = localTransform * rotMat;
}

Vector2 Transform::GetLocalScale()
{
	return Vector2(localTransform.GetScaleX(), localTransform.GetScaleY());
}

void Transform::SetLocalScale(Vector2 scale)
{
	localTransform.SetScaleX(scale.x);
	localTransform.SetScaleY(scale.y);
}

void Transform::Translate(Vector2 delta, bool moveLocal)
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

		localTransform = translationMatrix * localTransform;
	}
}

void Transform::UpdateGlobalMatrix()
{
	if (parent != nullptr)
		globalTransform = parent->GetGlobalTransform() * localTransform;
	else
		globalTransform = localTransform;

	up = globalTransform.GetUp().Normalised();
	right = globalTransform.GetRight().Normalised();

	for (int i = 0; i < children.size(); ++i)
	{
		children[i]->UpdateGlobalMatrix();
	}
}
