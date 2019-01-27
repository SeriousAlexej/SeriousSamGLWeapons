#ifndef ALEX_GLM_VEC2_H
#define ALEX_GLM_VEC2_H

namespace glm
  {
  struct vec2
    {
	vec2()
	{
		x = 0.0f;
		y = 0.0f;
	}
	
	vec2(const vec2& v)
	{
		x = v.x;
		y = v.y;
	}
	
	vec2(float ix, float iy)
	{
		x = ix;
		y = iy;
	}
	
	vec2(float f)
	{
		x = f;
		y = f;
	}
	
	vec2& operator*=(float f)
	{
		x *= f;
		y *= f;
		return *this;
	}
	
	vec2& operator/=(float f)
	{
		x /= f;
		y /= f;
		return *this;
	}
	
	vec2& operator*=(const vec2& v)
	{
		x *= v.x;
		y *= v.y;
		return *this;
	}
	
	vec2& operator/=(const vec2& v)
	{
		x /= v.x;
		y /= v.y;
		return *this;
	}
	
	vec2& operator+=(const vec2& v)
	{
		x += v.x;
		y += v.y;
		return *this;
	}
	
	vec2& operator-=(const vec2& v)
	{
		x -= v.x;
		y -= v.y;
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
		return 2;
	}

    float x, y;
    };
	
	static vec2 operator*(const vec2& a, float f)
	{
		return vec2(a) *= f;
	}
	
	static vec2 operator*(float f, const vec2& a)
	{
		return vec2(a) *= f;
	}
	
	static vec2 operator/(const vec2& a, float f)
	{
		return vec2(a) /= f;
	}
	
	static vec2 operator/(float f, const vec2& v)
	{
		return vec2(f) /= v;
	}
	
	static vec2 operator*(const vec2& a, const vec2& b)
	{
		return vec2(a) *= b;
	}
	
	static vec2 operator/(const vec2& a, const vec2& b)
	{
		return vec2(a) /= b;
	}
	
	static vec2 operator+(const vec2& a, const vec2& b)
	{
		return vec2(a) += b;
	}
	
	static vec2 operator-(const vec2& a, const vec2& b)
	{
		return vec2(a) -= b;
	}
  }

#endif
