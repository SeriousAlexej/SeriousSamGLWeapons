#ifndef ALEX_GLM_MATH_H
#define ALEX_GLM_MATH_H
#include <math.h>

namespace glm
  {
  static float atan(float f)
    {
    return ::atan(f);
    }

  static float tan(float f)
    {
    return ::tan(f);
    }

  static float radians(float f)
    {
    return f * static_cast<float>(0.01745329251994329576923690768489);
    }
	
	static float degrees(float f)
	{
		return f * static_cast<float>(57.295779513082320876798154814105);
	}
  }

#endif
