#pragma once
#include <cmath>

namespace MathMan
{
	static int sign(int x)
	{
		return (x > 0) - (x < 0);
	}
	#define DEG2RAD 0.01745329
	#define RAD2DEG	57.2957795
	#define PI 3.14159265359
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
		Vector2 operator -()
		{
			return Vector2(-this->x, -this->y);
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
			return sqrtf(SqrMagnitude());
		}

		float SqrMagnitude()
		{
			return x * x + y * y;
		}

		Vector2 Normalised()
		{
			return *this / this->Magnitude();
		}

		Vector2 RightAngle()
		{
			return Vector2(y, -x);
		}

		static Vector2 Scale(Vector2 a, Vector2 b)
		{
			return Vector2(a.x * b.x, a.y * b.y);
		}

		static float Dot(Vector2 a, Vector2 b)
		{
			return a.x * b.x + a.y * b.y;
		}

		static float Angle(Vector2 a, Vector2 b)
		{
			return (float)acos(Dot(a, b) / (a.Magnitude() * b.Magnitude()));
		}

		static float SignedAngle(Vector2 a, Vector2 b)
		{
			a = a.Normalised();
			b = b.Normalised();

			float angle = (float)acos(Dot(a, b));

			Vector2 rAngle = a.RightAngle();
			if (Dot(b, rAngle) < 0)
				angle = -angle;

			return angle;
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