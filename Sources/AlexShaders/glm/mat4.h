#ifndef ALEX_GLM_MAT4_H
#define ALEX_GLM_MAT4_H
#include "vec4.h"
#include "mat3.h"

namespace glm
  {
  struct mat4
    {
	mat4()
	{
	}
	
	mat4(float f)
	{
		for (size_t i = 0; i < 4; ++i)
			value[i][i] = f;
	}
	
	mat4(const mat3& m)
	{
		value[0] = vec4(m[0], 0.0f);
		value[1] = vec4(m[1], 0.0f);
		value[2] = vec4(m[2], 0.0f);
		value[3] = vec4(0.0f, 0.0f, 0.0f, 1.0f);
	}
		
    vec4& operator[](size_t i)
      {
      return value[i];
      }
	  
	const vec4& operator[](size_t i) const
	{
		return value[i];
	}

    vec4 value[4];
    };
	
	static vec4 operator*(const mat4& m, const vec4& v)
	{
		vec4 const Mov0(v[0]);
		vec4 const Mov1(v[1]);
		vec4 const Mul0 = m[0] * Mov0;
		vec4 const Mul1 = m[1] * Mov1;
		vec4 const Add0 = Mul0 + Mul1;
		vec4 const Mov2(v[2]);
		vec4 const Mov3(v[3]);
		vec4 const Mul2 = m[2] * Mov2;
		vec4 const Mul3 = m[3] * Mov3;
		vec4 const Add1 = Mul2 + Mul3;
		vec4 const Add2 = Add0 + Add1;
		return Add2;
	}
	
	static vec4 operator*(const vec4& v, const mat4& m)
	{
		return vec4(
			m[0][0] * v[0] + m[0][1] * v[1] + m[0][2] * v[2] + m[0][3] * v[3],
			m[1][0] * v[0] + m[1][1] * v[1] + m[1][2] * v[2] + m[1][3] * v[3],
			m[2][0] * v[0] + m[2][1] * v[1] + m[2][2] * v[2] + m[2][3] * v[3],
			m[3][0] * v[0] + m[3][1] * v[1] + m[3][2] * v[2] + m[3][3] * v[3]);
	}
	
	static mat4 operator*(const mat4& m1, const mat4& m2)
	{
		vec4 const SrcA0 = m1[0];
		vec4 const SrcA1 = m1[1];
		vec4 const SrcA2 = m1[2];
		vec4 const SrcA3 = m1[3];

		vec4 const SrcB0 = m2[0];
		vec4 const SrcB1 = m2[1];
		vec4 const SrcB2 = m2[2];
		vec4 const SrcB3 = m2[3];

		mat4 Result(0.0f);
		Result[0] = SrcA0 * SrcB0[0] + SrcA1 * SrcB0[1] + SrcA2 * SrcB0[2] + SrcA3 * SrcB0[3];
		Result[1] = SrcA0 * SrcB1[0] + SrcA1 * SrcB1[1] + SrcA2 * SrcB1[2] + SrcA3 * SrcB1[3];
		Result[2] = SrcA0 * SrcB2[0] + SrcA1 * SrcB2[1] + SrcA2 * SrcB2[2] + SrcA3 * SrcB2[3];
		Result[3] = SrcA0 * SrcB3[0] + SrcA1 * SrcB3[1] + SrcA2 * SrcB3[2] + SrcA3 * SrcB3[3];
		return Result;
	}
  }

#endif
