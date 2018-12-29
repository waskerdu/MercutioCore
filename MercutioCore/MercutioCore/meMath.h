#pragma once

#include <glm\glm.hpp>
#include <glm\gtc\matrix_transform.hpp>
#include <glm\gtc\type_ptr.hpp>
#include <math.h>

const double pi = 3.1415926;

/*struct Quat
{
	glm::quat quat;
	Quat Normalized()
	{
		Quat outQuat;
		outQuat.quat = glm::normalize(quat);
		return outQuat;
	}
	void Normalize()
	{
		quat = glm::normalize(quat);
	}
	double Mag()
	{
		return glm::length(quat);
	}
	void SetMag(double scalar)
	{
		Normalize();
		quat *= scalar;
	}
};

Quat operator+(const Quat vec, const Quat vec1)
{
	return Quat(vec + vec1);
}
Quat operator-(const Quat vec, const Quat vec1)
{
	return Quat(vec - vec1);
}
Quat operator*(const Quat vec, const Quat vec1)
{
	return Quat(vec * vec1);
}
Quat operator/(const Quat vec, const Quat vec1)
{
	return Quat(vec / vec1);
}

struct Vec2
{
	glm::vec2 vector;
	Vec2()
	{
		vector = glm::vec2();
	}
	Vec2(glm::vec2 vec)
	{
		vector = vec;
	}
	Vec2(double scalar, double scalar1)
	{
		vector = glm::vec2(scalar, scalar1);
	}
	Vec2(double scalar)
	{
		vector = glm::vec2(scalar);
	}
	Vec2 Normalized()
	{
		Vec2 outVec;
		outVec.vector = glm::normalize(vector);
		return outVec;
	}
	void Normalize()
	{
		vector = glm::normalize(vector);
	}
	double Dot(Vec2 vec)
	{
		return glm::dot(vector, vec.vector);
	}
	double Mag()
	{
		return glm::length(vector);
	}
	void SetMag(double scalar)
	{
		Normalize();
		vector *= scalar;
	}
};

Vec2 operator+(const Vec2 vec, const Vec2 vec1)
{
	return Vec2(vec + vec1);
}
Vec2 operator-(const Vec2 vec, const Vec2 vec1)
{
	return Vec2(vec - vec1);
}
Vec2 operator*(const Vec2 vec, const Vec2 vec1)
{
	return Vec2(vec * vec1);
}
Vec2 operator/(const Vec2 vec, const Vec2 vec1)
{
	return Vec2(vec / vec1);
}

struct Vec3
{
	glm::vec3 vector;
	Vec3()
	{
		vector = glm::vec3();
	}
	Vec3(glm::vec3 vec)
	{
		vector = vec;
	}
	Vec3(Vec2 vec, double scalar = 0)
	{
		vector = glm::vec3(vec.vector, scalar);
	}
	Vec3(double scalar, double scalar1, double scalar2)
	{
		vector = glm::vec3(scalar, scalar1, scalar2);
	}
	Vec3(double scalar)
	{
		vector = glm::vec3(scalar);
	}
	Vec3 Normalized()
	{
		Vec3 outVec;
		outVec.vector = glm::normalize(vector);
		return outVec;
	}
	void Normalize()
	{
		vector = glm::normalize(vector);
	}
	double Dot(Vec3 vec)
	{
		return glm::dot(vector, vec.vector);
	}
	Vec3 Cross(Vec3 vec)
	{
		Vec3 outVec;
		outVec.vector = glm::cross(vec.vector, vector);
	}
	double Mag()
	{
		return glm::length(vector);
	}
	void SetMag(double scalar)
	{
		Normalize();
		vector *= scalar;
	}
};

Vec3 operator+(const Vec3 vec, const Vec3 vec1)
{
	return Vec3(vec + vec1);
}
Vec3 operator-(const Vec3 vec, const Vec3 vec1)
{
	return Vec3(vec - vec1);
}
Vec3 operator*(const Vec3 vec, const Vec3 vec1)
{
	return Vec3(vec * vec1);
}
Vec3 operator/(const Vec3 vec, const Vec3 vec1)
{
	return Vec3(vec / vec1);
}

struct Vec4
{
	glm::vec4 vector;
	Vec4()
	{
		vector = glm::vec4();
	}
	Vec4(glm::vec4 vec)
	{
		vector = vec;
	}
	Vec4(Vec3 vec, double scalar = 0)
	{
		vector = glm::vec4(vec.vector, scalar);
	}
	Vec4(Vec2 vec, double scalar = 0, double scalar1 = 0)
	{
		vector = glm::vec4(vec.vector, scalar, scalar1);
	}
	Vec4(Vec2 vec, Vec2 vec1)
	{
		vector = glm::vec4(vec.vector, vec1.vector);
	}
	Vec4(double scalar, double scalar1, double scalar2, double scalar3)
	{
		vector = glm::vec4(scalar, scalar1, scalar2, scalar3);
	}
	Vec4(double scalar)
	{
		vector = glm::vec4((float)scalar);
	}
	Vec4 Normalized()
	{
		Vec4 outVec;
		outVec.vector = glm::normalize(vector);
		return outVec;
	}
	void Normalize()
	{
		vector = glm::normalize(vector);
	}
	double Dot(Vec4 vec)
	{
		return glm::dot(vector, vec.vector);
	}
	double Mag()
	{
		return glm::length(vector);
	}
	void SetMag(double scalar)
	{
		Normalize();
		vector *= scalar;
	}
};

Vec4 operator+(const Vec4 vec, const Vec4 vec1)
{
	return Vec4(vec + vec1);
}
Vec4 operator-(const Vec4 vec, const Vec4 vec1)
{
	return Vec4(vec - vec1);
}
Vec4 operator*(const Vec4 vec, const Vec4 vec1)
{
	return Vec4(vec * vec1);
}
Vec4 operator/(const Vec4 vec, const Vec4 vec1)
{
	return Vec4(vec / vec1);
}/**/