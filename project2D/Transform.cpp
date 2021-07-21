#include "Transform.h"
#include "Entity.h"

Transform::~Transform()
{
	//Remove this object as a child of the parent
	SetParent(nullptr);
}

Transform* Transform::CloneTo(Entity* ent)
{
	//Don't clone, just copy data since transform already exists
	Transform* newTransform = ent->GetTransform();
	newTransform->localTransform = this->localTransform;
	newTransform->globalTransform = this->globalTransform;
	newTransform->SetParent(this->parent);

	return newTransform;
}

Transform* Transform::GetParent()
{
	return parent;
}

void Transform::SetParent(Transform* transform)
{
	if (parent != nullptr)
		parent->_RemoveChild(this);

	parent = transform;

	if (parent != nullptr)
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

std::vector<Transform*>* Transform::GetChildrenList()
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

size_t Transform::GetChildCount()
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
	//Minus current globalRotation before adding new rotation
	SetLocalRotation(-parent->GetGlobalRotation() + radians);
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
		//Create translation matrix scaled by the global transform's inverse matrix, moving the object in world space
		Matrix3 translationMatrix = Matrix3::Identity();
		translationMatrix.m[6] = delta.x;
		translationMatrix.m[7] = delta.y;
		translationMatrix = translationMatrix * parent->globalTransform.Inverse();

		localTransform = translationMatrix * localTransform;
	}
}

void Transform::Rotate(float radians)
{
	Matrix3 rotMat = Matrix3::Identity();
	rotMat.SetRotateZ(radians);

	localTransform = localTransform * rotMat;
}

void Transform::SetUp(Vector2 direction)
{
	float newRotation = atan2(direction.y, direction.x);

	if (isnan<float>(newRotation))
		newRotation = 0;

	SetGlobalRotation(newRotation);
}

void Transform::LookAt(Vector2 point)
{
	Vector2 objectToPoint = point - GetGlobalPosition();

	objectToPoint = objectToPoint.Normalised();

	SetUp(objectToPoint);
}

void Transform::UpdateGlobalMatrix()
{
	//Don't multiply by parent global if there is none
	if (parent != nullptr)
		globalTransform = parent->GetGlobalTransform() * localTransform;
	else
		globalTransform = localTransform;

	//Update up/right
	up = globalTransform.GetUp().Normalised();
	right = globalTransform.GetRight().Normalised();

	//Update child matrices
	for (int i = 0; i < children.size(); ++i)
	{
		children[i]->UpdateGlobalMatrix();
	}
}
