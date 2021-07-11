#pragma once
#include <cmath>

namespace MathMan
{
	struct Vector2
	{
		float x, y;

		Vector2(float x = 0, float y = 0)
		{
			this->x = x;
			this->y = y;
		}
		Vector2(Vector2& vec)
		{
			this->x = vec.x;
			this->y = vec.y;
		}

		~Vector2() { }

		Vector2 operator +(Vector2 other)
		{
			return Vector2(this->x + other.x, this->y + other.y);
		}

		Vector2 operator -(Vector2 other)
		{
			return Vector2(this->x - other.x, this->y - other.y);
		}

		Vector2 operator /(float scalar)
		{
			return Vector2(this->x / scalar, this->y / scalar);
		}

		Vector2 operator *(float scalar)
		{
			return Vector2(this->x * scalar, this->y * scalar);
		}

		Vector2& operator +=(Vector2 vec)
		{
			x = x + vec.x;
			y = y + vec.y;
			return *this;
		}
		Vector2& operator -=(Vector2 vec)
		{
			x = x - vec.x;
			y = y - vec.y;
			return *this;
		}
		Vector2& operator *=(float scalar)
		{
			x = x * scalar;
			y = y * scalar;
			return *this;
		}
		Vector2& operator /=(float scalar)
		{
			x = x / scalar;
			y = y / scalar;
			return *this;
		}

		float Magnitude()
		{
			return sqrtf(x * x + y * y);
		}

		Vector2 Normalised()
		{
			return *this / this->Magnitude();
		}

		static Vector2 Scale(Vector2 a, Vector2 b)
		{
			return Vector2(a.x * b.x, a.y * b.y);
		}

		static Vector2 One()
		{
			return Vector2(1, 1);
		}

		static Vector2 Up()
		{
			return Vector2(0, 1);
		}

		static Vector2 Right()
		{
			return Vector2(1, 0);
		}
	};
}