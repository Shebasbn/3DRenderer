#pragma once

#include <math.h>
#include <vector>

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

		/*Vec3_t Rotate(Vec3_t v) const
		{
			Vec3_t ResultVec;
			ResultVec.X = Rota
		}*/

		Vec3_t RotateX(float angle) const
		{
			return Vec3_t(
				X,
				Y * cos(angle) + Z * sin(angle),
				Z * cos(angle) - Y * sin(angle));
		}

		Vec3_t RotateY(float angle) const
		{
			return Vec3_t(
				X * cos(angle) - Z * sin(angle),
				Y,
				Z * cos(angle) + X * sin(angle));
		}

		Vec3_t RotateZ(float angle) const
		{
			return Vec3_t(
				X * cos(angle) - Y * sin(angle),
				Y * cos(angle) + X * sin(angle),
				Z);
		}

	public:
		float X;
		float Y;
		float Z;
	};

	
}