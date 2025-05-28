#pragma once

//class Vector
//{
//public:
//	virtual void Add() = 0;
//	virtual void Subtract() = 0;
//	virtual scale
//};
namespace Renderer3D
{
	class Vec2_t
	{
	public:
		Vec2_t(float x = 0, float y = 0) :
			X{x},
			Y{y}
		{
		}

		Vec2_t Add(const Vec2_t& v) const { return Vec2_t(X + v.X, Y + v.Y); }
		//Vec2_t operator+(const Vec2_t& v1, const Vec2_t& v2) const { return v1.Add(v2); }

	public:
		float X;
		float Y;
	};

	class Vec3_t
	{
	public:
		Vec3_t(float x = 0, float y = 0, float z = 0) :
			X{ x },
			Y{ y },
			Z{ z }
		{
		}

		Vec3_t Add(const Vec3_t& v) const { return Vec3_t(X + v.X, Y + v.Y, Y + v.Z); }
		//Vec3_t operator+(const Vec3_t& v1, const Vec3_t& v2) const { return v1.Add(v2); }

	public:
		float X;
		float Y;
		float Z;
	};
}