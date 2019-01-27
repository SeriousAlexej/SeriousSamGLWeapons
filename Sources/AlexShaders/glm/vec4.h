#ifndef ALEX_GLM_VEC4_H
#define ALEX_GLM_VEC4_H
#include "vec3.h"

namespace glm
  {
  struct vec4
    {
	vec4()
	{
		x = 0.0f;
		y = 0.0f;
		z = 0.0f;
		w = 0.0f;
	}
	
	vec4(const vec4& v)
	{
		x = v.x;
		y = v.y;
		z = v.z;
		w = v.w;
	}
	
	vec4(const vec3& v, float f)
	{
		x = v.x;
		y = v.y;
		z = v.z;
		w = f;
	}
	
	vec4(float ix, float iy, float iz, float iw)
	{
		x = ix;
		y = iy;
		z = iz;
		w = iw;
	}
	
	vec4(float f)
	{
		x = f;
		y = f;
		z = f;
		w = f;
	}
	
	vec4& operator*=(float f)
	{
		x *= f;
		y *= f;
		z *= f;
		w *= f;
		return *this;
	}
	
	vec4& operator/=(float f)
	{
		x /= f;
		y /= f;
		z /= f;
		w /= f;
		return *this;
	}
	
	vec4& operator*=(const vec4& v)
	{
		x *= v.x;
		y *= v.y;
		z *= v.z;
		w *= v.w;
		return *this;
	}
	
	vec4& operator/=(const vec4& v)
	{
		x /= v.x;
		y /= v.y;
		z /= v.z;
		w /= v.w;
		return *this;
	}
	
	vec4& operator+=(const vec4& v)
	{
		x += v.x;
		y += v.y;
		z += v.z;
		w += v.w;
		return *this;
	}
	
	vec4& operator-=(const vec4& v)
	{
		x -= v.x;
		y -= v.y;
		z -= v.z;
		w -= v.w;
		return *this;
	}

	operator vec3()
	{
		return vec3(x, y, z);
	}
	
    float& operator[](size_t i)
      {
      return (&x)[i];
      }
	
    float operator[](size_t i) const
      {
      return (&x)[i];
      }
	  
	static size_t length()
	{
		return 4;
	}

    float x, y, z, w;
    };
	
	static vec4 operator*(const vec4& a, float f)
	{
		return vec4(a) *= f;
	}
	
	static vec4 operator*(float f, const vec4& a)
	{
		return vec4(a) *= f;
	}
	
	static vec4 operator/(const vec4& a, float f)
	{
		return vec4(a) /= f;
	}
	
	static vec4 operator/(float f, const vec4& v)
	{
		return vec4(f) /= v;
	}
	
	static vec4 operator*(const vec4& a, const vec4& b)
	{
		return vec4(a) *= b;
	}
	
	static vec4 operator/(const vec4& a, const vec4& b)
	{
		return vec4(a) /= b;
	}
	
	static vec4 operator+(const vec4& a, const vec4& b)
	{
		return vec4(a) += b;
	}
	
	static vec4 operator-(const vec4& a, const vec4& b)
	{
		return vec4(a) -= b;
	}
  }

#endif
