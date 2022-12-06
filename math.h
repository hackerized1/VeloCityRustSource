#pragma once
#include <cmath>
#include <math.h>
#include <vector>

namespace math
{
	struct vector4
	{
		float x, y, z, w;
	};

	struct matrix3x4
	{
		vector4 vec0;
		vector4 vec1;
		vector4 vec2;
	};

	class vector2
	{
	public:
		vector2()
		{
			x = y = 0.f;
		}

		vector2(float fx, float fy)
		{
			x = fx;
			y = fy;
		}

		float x, y;

		vector2 operator/(float input) const
		{
			return vector2{ x / input, y / input };
		}

		vector2 operator*(float input) const
		{
			return vector2{ x * input, y * input };
		}

		vector2 operator+(const vector2& input) const
		{
			return vector2{ x + input.x, y + input.y };
		}

		vector2 operator-(const vector2& input) const
		{
			return vector2{ x - input.x, y - input.y };
		}

		float length_sqr() const
		{
			return (x * x) + (y * y);
		}

		float length() const
		{
			return std::sqrt(length_sqr());
		}

		float distance(const vector2& second) const
		{
			return sqrtf(pow(x - second.x, 2) + pow(y - second.y, 2));
		}

		vector2& operator/=(float input)
		{
			x /= input;
			y /= input;
			return *this;
		}

		vector2& operator*=(float input)
		{
			x *= input;
			y *= input;
			return *this;
		}


		vector2& operator-=(const vector2& v)
		{
			x -= v.x;
			y -= v.y;

			return *this;
		}



		bool operator==(const vector2& input) const
		{
			return x == input.x && y == input.y;
		}


		bool empty() const
		{
			return x == 0.f && y == 0.f;
		}

		float dot_product(vector2 input) const
		{
			return (this->x * input.x) + (this->y * input.y);
		}

	};

	class vector3
	{
	public:
		vector3()
		{
			x = y = z = 0.f;
		}

		vector3(float fx, float fy, float fz)
		{
			x = fx;
			y = fy;
			z = fz;
		}

		float x, y, z;



		vector3 operator/(float input) const
		{
			return vector3{ x / input, y / input, z / input };
		}

		vector3 operator*(float input) const
		{
			return vector3{ x * input, y * input, z * input };
		}

		vector3 operator+(const vector3& input) const
		{
			return vector3{ x + input.x, y + input.y, z + input.z };
		}

		vector3 operator-(const vector3& input) const
		{
			return vector3{ x - input.x, y - input.y, z - input.z };
		}

		float length_sqr() const
		{
			return (x * x) + (y * y) + (z * z);
		}

		float length() const
		{
			return std::sqrt(length_sqr());
		}



		float distance(const vector3& second) const
		{
			return sqrtf(pow(x - second.x, 2) + pow(y - second.y, 2) + pow(z - second.z, 2));
		}

		vector3& operator/=(float input)
		{
			x /= input;
			y /= input;
			z /= input;
			return *this;
		}

		vector3& operator*=(float input)
		{
			x *= input;
			y *= input;
			z *= input;
			return *this;
		}


		vector3& operator-=(const vector3& v)
		{
			x -= v.x;
			y -= v.y;
			z -= v.z;

			return *this;
		}



		bool operator==(const vector3& input) const
		{
			return x == input.x && y == input.y && z == input.z;
		}


		bool empty() const
		{
			return x == 0.f && y == 0.f && z == 0.f;
		}

		float dot_product(vector3 input) const
		{
			return (this->x * input.x) + (this->y * input.y) + (this->z * input.z);
		}

	};

	class matrix4x4
	{
	public:
		inline float* Base()
		{
			return &m[0][0];
		}

		inline const float* Base() const
		{
			return &m[0][0];
		}

		inline float* operator[](int i)
		{
			return m[i];
		}

		inline const float* operator[](int i) const
		{
			return m[i];
		}
	public:

		inline matrix4x4()
		{
		}

		inline matrix4x4(
			float m00, float m01, float m02, float m03,
			float m10, float m11, float m12, float m13,
			float m20, float m21, float m22, float m23,
			float m30, float m31, float m32, float m33)
		{
			Init(
				m00, m01, m02, m03,
				m10, m11, m12, m13,
				m20, m21, m22, m23,
				m30, m31, m32, m33
			);
		}

		inline void Init(
			float m00, float m01, float m02, float m03,
			float m10, float m11, float m12, float m13,
			float m20, float m21, float m22, float m23,
			float m30, float m31, float m32, float m33
		)
		{
			m[0][0] = m00;
			m[0][1] = m01;
			m[0][2] = m02;
			m[0][3] = m03;

			m[1][0] = m10;
			m[1][1] = m11;
			m[1][2] = m12;
			m[1][3] = m13;

			m[2][0] = m20;
			m[2][1] = m21;
			m[2][2] = m22;
			m[2][3] = m23;

			m[3][0] = m30;
			m[3][1] = m31;
			m[3][2] = m32;
			m[3][3] = m33;
		}

		matrix4x4 transpose() const
		{
			return matrix4x4(
				m[0][0], m[1][0], m[2][0], m[3][0],
				m[0][1], m[1][1], m[2][1], m[3][1],
				m[0][2], m[1][2], m[2][2], m[3][2],
				m[0][3], m[1][3], m[2][3], m[3][3]);
		}

		float m[4][4];
	};

}
