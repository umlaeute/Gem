// p_vector.h
//
// Copyright 1997 by Jonathan P. Leech
// Modifications Copyright 1997-1999 by David K. McAllister
//
// A simple 3D float vector class for internal use by the particle systems.

#ifndef particle_vector_h
#define particle_vector_h

#include <math.h>

#ifdef __ppc__
#include "Base/GemFuncUtil.h"
#undef sqrt
#define sqrt fast_sqrtf
#define sqrtf fast_sqrtf
#endif

#ifndef M_PI
#define M_PI 3.1415926535897932384626433f
#endif

#ifdef __WIN32__
#define drand48() (((float) rand())/((float) RAND_MAX))
#define srand48(x) srand(x)

// This is because their stupid compiler thinks it's smart.
#define inline __forceinline
#endif

#ifdef __APPLE__
#define drand48() (((float) rand())/((float) RAND_MAX))
#define srand48(x) srand(x)
#include "__APPLE___math.h"
#endif

#ifdef __linux__
// jmz: ? i don't understand my compiler ...
#ifndef sqrtf
#define sqrtf sqrt
#endif
#endif
class pVector
{
public:
	float x, y, z;
	
	inline pVector(float ax, float ay, float az) : x(ax), y(ay), z(az)
	{
		//x = ax; y = ay; z = az;
	}
	
	inline pVector() {}
	
	inline float length()
	{
		return sqrtf(x*x+y*y+z*z);
	}
	
	inline float length2()
	{
		return (x*x+y*y+z*z);
	}
	
	inline float normalize()
	{
		float onel = 1.0f / sqrtf(x*x+y*y+z*z);
		x *= onel;
		y *= onel;
		z *= onel;
		
		return onel;
	}
	
	inline float operator*(const pVector &a) const
	{
		return x*a.x + y*a.y + z*a.z;
	}
	
	inline pVector operator*(const float s) const
	{
		return pVector(x*s, y*s, z*s);
	}
	
	inline pVector operator/(const float s) const
	{
		float invs = 1.0f / s;
		return pVector(x*invs, y*invs, z*invs);
	}
	
	inline pVector operator+(const pVector& a) const
	{
		return pVector(x+a.x, y+a.y, z+a.z);
	}
	
	inline pVector operator-(const pVector& a) const
	{
		return pVector(x-a.x, y-a.y, z-a.z);
	}
	
	inline pVector operator-()
	{
		x = -x;
		y = -y;
		z = -z;
		return *this;
	}
	
	inline pVector& operator+=(const pVector& a)
	{
		x += a.x;
		y += a.y;
		z += a.z;
		return *this;
	}
	
	inline pVector& operator-=(const pVector& a)
	{
		x -= a.x;
		y -= a.y;
		z -= a.z;
		return *this;
	}
	
	inline pVector& operator*=(const float a)
	{
		x *= a;
		y *= a;
		z *= a;
		return *this;
	}
	
	inline pVector& operator/=(const float a)
	{
		float b = 1.0f / a;
		x *= b;
		y *= b;
		z *= b;
		return *this;
	}
	
	inline pVector& operator=(const pVector& a)
	{
		x = a.x;
		y = a.y;
		z = a.z;
		return *this;
	}
	
	inline pVector operator^(const pVector& b)
	{
		return pVector(
			y*b.z-z*b.y,
			z*b.x-x*b.z,
			x*b.y-y*b.x);
	}
};

#endif
