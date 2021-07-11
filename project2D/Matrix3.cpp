#pragma once

#include "Matrix3.h"

namespace MathMan
{
	Matrix3::Matrix3()
	{
		m[0] = 1;
		m[1] = 0;
		m[2] = 0;
		m[3] = 0;
		m[4] = 1;
		m[5] = 0;
		m[6] = 0;
		m[7] = 0;
		m[8] = 1;
	}

	Matrix3::Matrix3(float m0, float m1, float m2, float m3, float m4, float m5, float m6, float m7, float m8)
	{
		m[0] = m0;
		m[1] = m1;
		m[2] = m2;
		m[3] = m3;
		m[4] = m4;
		m[5] = m5;
		m[6] = m6;
		m[7] = m7;
		m[8] = m8;
	}

	Matrix3 Matrix3::operator *(Matrix3 other)
	{
		//Matrix multiplication ;)
		Matrix3 mat = Matrix3();
		mat.m[0] = m[0] * other.m[0] + m[3] * other.m[1] + m[6] * other.m[2];
		mat.m[3] = m[0] * other.m[3] + m[3] * other.m[4] + m[6] * other.m[5];
		mat.m[6] = m[0] * other.m[6] + m[3] * other.m[7] + m[6] * other.m[8];
		mat.m[1] = m[1] * other.m[0] + m[4] * other.m[1] + m[7] * other.m[2];
		mat.m[4] = m[1] * other.m[3] + m[4] * other.m[4] + m[7] * other.m[5];
		mat.m[7] = m[1] * other.m[6] + m[4] * other.m[7] + m[7] * other.m[8];
		mat.m[2] = m[2] * other.m[0] + m[5] * other.m[1] + m[8] * other.m[2];
		mat.m[5] = m[2] * other.m[3] + m[5] * other.m[4] + m[8] * other.m[5];
		mat.m[8] = m[2] * other.m[6] + m[5] * other.m[7] + m[8] * other.m[8];

		return mat;
	}

	Vector2 Matrix3::operator *(Vector2 vec)
	{
		return Vector2(m[0] * vec.x + m[3] * vec.y, m[1] * vec.x + m[4] * vec.y);
	}

	Matrix3 Matrix3::operator *(float scalar)
	{
		return Matrix3(m[0] * scalar, m[1] * scalar, m[2] * scalar, m[3] * scalar, m[4] * scalar, m[5] * scalar, m[6] * scalar, m[7] * scalar, m[8] * scalar);
	}

	Matrix3 Matrix3::Inverse()
	{
		//Gets matrix of minors, negates every second element, moves each element diagonally to the opposite side
		Matrix3 minors = Matrix3();
		minors.m[0] = (m[4] * m[8] - m[7] * m[5]);	m[3] = (m[1] * m[8] - m[7] * m[2]); m[6] = (m[1] * m[5] - m[4] * m[2]);
		minors.m[1] = (m[3] * m[8] - m[6] * m[5]);	m[4] = (m[0] * m[8] - m[6] * m[2]); m[7] = (m[0] * m[5] - m[3] * m[2]);
		minors.m[2] = (m[3] * m[7] - m[4] * m[6]);	m[5] = (m[0] * m[7] - m[6] * m[1]); m[8] = (m[0] * m[4] - m[3] * m[1]);

		Matrix3 adjugate = Matrix3(minors.m[0], -minors.m[3], minors.m[6], -minors.m[1], minors.m[4], -minors.m[7], minors.m[2], -minors.m[5], minors.m[8]);

		float determinant = (m[0] * minors.m[0]) - (m[3] * minors.m[3]) + (m[6] * minors.m[6]);

		if (determinant == 0)
			throw ("Cannot divide adjugate by 0");

		return adjugate * (1 / determinant);
	}

	Vector2 Matrix3::GetRight()
	{
		return Vector2(m[0], m[1]);
	}

	Vector2 Matrix3::GetUp()
	{
		return Vector2(m[3], m[4]);
	}

	float Matrix3::GetScaleX()
	{
		return GetRight().Magnitude();
	}

	float Matrix3::GetScaleY()
	{
		return GetUp().Magnitude();
	}

	void Matrix3::SetScaleX(float scale)
	{
		Vector2 right = GetRight().Normalised() * scale;

		m[0] = right.x;
		m[1] = right.y;
	}

	void Matrix3::SetScaleY(float scale)
	{
		Vector2 up = GetUp().Normalised() * scale;

		m[3] = up.x;
		m[4] = up.y;
	}

	void Matrix3::SetRotateZ(float radians)
	{
		float sin = std::sin(radians);
		float cos = std::cos(radians);
		m[0] = cos;
		m[1] = sin;
		m[3] = -sin;
		m[4] = cos;
		m[8] = 1;
	}

	void Matrix3::SetRotation(float radians)
	{
		Vector2 scale = Vector2(GetScaleX(), GetScaleY());

		//Reset rotation axes to 0
		m[0] = 1;
		m[1] = 0;
		m[3] = 0;
		m[4] = 1;

		//Creates a new rotation matrix
		Matrix3 rotMat = Identity();
		rotMat.SetRotateZ(radians);

		//Scale x and y axis by scale x and scale y
		rotMat.m[0] *= scale.x;
		rotMat.m[1] *= scale.x;
		rotMat.m[3] *= scale.y;
		rotMat.m[4] *= scale.y;

		*this = *this * rotMat;
	}

	float Matrix3::GetRotation()
	{
		Vector2 right = GetRight();
		return std::atan2(right.y, right.x);
	}

	void Matrix3::SetIdentity()
	{
		m[0] = 1;
		m[4] = 1;
		m[8] = 1;
	}
}