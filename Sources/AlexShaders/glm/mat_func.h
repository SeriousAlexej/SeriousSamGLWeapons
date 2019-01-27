#ifndef ALEX_GLM_MATFUNCS_H
#define ALEX_GLM_MATFUNCS_H
#include <math.h>
#include "mat3.h"
#include "mat4.h"
#include "vec3.h"

namespace glm
{
	static mat3 transpose(const mat3& m)
	{
		mat3 result(0.0f);
		result[0][0] = m[0][0];
		result[0][1] = m[1][0];
		result[0][2] = m[2][0];

		result[1][0] = m[0][1];
		result[1][1] = m[1][1];
		result[1][2] = m[2][1];

		result[2][0] = m[0][2];
		result[2][1] = m[1][2];
		result[2][2] = m[2][2];
		return result;
	}
	
	static mat4 transpose(const mat4& m)
	{
		mat4 result(0.0f);
		result[0][0] = m[0][0];
		result[0][1] = m[1][0];
		result[0][2] = m[2][0];
		result[0][3] = m[3][0];

		result[1][0] = m[0][1];
		result[1][1] = m[1][1];
		result[1][2] = m[2][1];
		result[1][3] = m[3][1];

		result[2][0] = m[0][2];
		result[2][1] = m[1][2];
		result[2][2] = m[2][2];
		result[2][3] = m[3][2];

		result[3][0] = m[0][3];
		result[3][1] = m[1][3];
		result[3][2] = m[2][3];
		result[3][3] = m[3][3];
		return result;
	}
	
	static float clamp(float x, float minVal, float maxVal)
	{
		return min(max(x, minVal), maxVal);
	}
	
	static float dot(const vec3& x, const vec3& y)
	{
		vec3 tmp(x * y);
		return tmp.x + tmp.y + tmp.z;
	}
	
	static float dot(const vec4& x, const vec4& y)
	{
		vec4 tmp(x * y);
		return (tmp.x + tmp.y) + (tmp.z + tmp.w);
	}
		
	static vec3 normalize(const vec3& v)
	{
		return v / ::sqrtf(dot(v, v));
	}
	
	static vec4 normalize(const vec4& v)
	{
		return v / ::sqrtf(dot(v, v));
	}
	
	static vec3 cross(const vec3& x, const vec3& y)
	{
		return vec3(
			x.y * y.z - y.y * x.z,
			x.z * y.x - y.z * x.x,
			x.x * y.y - y.x * x.y);
	}
	
	static mat4 perspective(float fovy, float aspect, float zNear, float zFar)
	{
		float const tanHalfFovy = ::tan(fovy * 0.5f);

		mat4 Result(0.0f);
		Result[0][0] = 1.0f / (aspect * tanHalfFovy);
		Result[1][1] = 1.0f / (tanHalfFovy);
		Result[2][3] = -1.0f;
		Result[2][2] = - (zFar + zNear) / (zFar - zNear);
		Result[3][2] = - (2.0f * zFar * zNear) / (zFar - zNear);

		return Result;
	}
}

#endif
