#pragma once
#include <array>
#include <cmath>
#include "Vector2.h"

namespace MathMan
{
	#define DEG2RAD 0.01745329
	#define RAD2DEG	57.2957795
	#define PI 3.14159265359


	struct Matrix3
	{
		//	________Structure________
		//	|	0	|	3	|	6	|
		//	|_______|_______|_______|
		//	|	1	|	4	|	7	|
		//	|_______|_______|_______|
		//	|	2	|	5	|	8	|
		//	|_______|_______|_______|

		static Matrix3 Identity()
		{
			return Matrix3();
		}

		std::array<float, 9> m;

		Matrix3();

		Matrix3(float m0, float m1, float m2, float m3, float m4, float m5, float m6, float m7, float m8);

		Matrix3 operator *(Matrix3 other);
		
		Vector2 operator *(Vector2 vec);

		Matrix3 operator *(float scalar);

		Matrix3 Inverse();

		Vector2 GetRight();

		Vector2 GetUp();

		float GetScaleX();

		float GetScaleY();

		void SetScaleX(float scale);

		void SetScaleY(float scale);

		void SetRotateZ(float radians);

		void SetRotation(float radians);

		float GetRotation();

		void SetIdentity();
	};
}