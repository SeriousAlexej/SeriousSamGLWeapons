#ifndef ALEX_GLM_VEC3_H
#define ALEX_GLM_VEC3_H

namespace glm
  {
  struct vec3
    {
	vec3()
	{
		x = 0.0f;
		y = 0.0f;
		z = 0.0f;
	}
	
	vec3(const vec3& v)
	{
		x = v.x;
		y = v.y;
		z = v.z;
	}
	
	vec3(float ix, float iy, float iz)
	{
		x = ix;
		y = iy;
		z = iz;
	}
	
	vec3(float f)
	{
		x = f;
		y = f;
		z = f;
	}
	
	vec3& operator*=(float f)
	{
		x *= f;
		y *= f;
		z *= f;
		return *this;
	}
	
	vec3& operator/=(float f)
	{
		x /= f;
		y /= f;
		z /= f;
		return *this;
	}
	
	vec3& operator*=(const vec3& v)
	{
		x *= v.x;
		y *= v.y;
		z *= v.z;
		return *this;
	}
	
	vec3& operator/=(const vec3& v)
	{
		x /= v.x;
		y /= v.y;
		z /= v.z;
		return *this;
	}
	
	vec3& operator+=(const vec3& v)
	{
		x += v.x;
		y += v.y;
		z += v.z;
		return *this;
	}
	
	vec3& operator-=(const vec3& v)
	{
		x -= v.x;
		y -= v.y;
		z -= v.z;
		return *this;
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
		return 3;
	}

    float x, y, z;
    };
	
	static vec3 operator*(const vec3& a, float f)
	{
		return vec3(a) *= f;
	}
	
	static vec3 operator*(float f, const vec3& a)
	{
		return vec3(a) *= f;
	}
	
	static vec3 operator/(const vec3& a, float f)
	{
		return vec3(a) /= f;
	}
	
	static vec3 operator/(float f, const vec3& v)
	{
		return vec3(f) /= v;
	}
	
	static vec3 operator*(const vec3& a, const vec3& b)
	{
		return vec3(a) *= b;
	}
	
	static vec3 operator/(const vec3& a, const vec3& b)
	{
		return vec3(a) /= b;
	}
	
	static vec3 operator+(const vec3& a, const vec3& b)
	{
		return vec3(a) += b;
	}
	
	static vec3 operator-(const vec3& a, const vec3& b)
	{
		return vec3(a) -= b;
	}
  }

#endif
