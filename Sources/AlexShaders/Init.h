/*
    SeriousSamGLWeapons - a free software for rendering Serious Sam weapon models with OpenGL 3.3 shaders
    Copyright (C) 2019 Oleksii Sierov (seriousalexej@gmail.com)
	
    SeriousSamGLWeapons is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.
    SeriousSamGLWeapons is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU Lesser General Public License for more details.
    You should have received a copy of the GNU Lesser General Public License
    along with SeriousSamGLWeapons.  If not, see <http://www.gnu.org/licenses/>.
*/
#ifndef ALEX_SHADERS_INIT_H
#define ALEX_SHADERS_INIT_H
#include "AlexShaders.h"

class CModelInstance;

ALEX_API bool AlexShaders_Initialize();
ALEX_API void AlexShaders_Finalize();
ALEX_API int  AlexShaders_RegisterRenderSession(int surfaces = 1);
ALEX_API void AlexShaders_SetRenderSession(int renderSession);
ALEX_API void AlexShaders_ClearAllSessions();
ALEX_API void AlexShaders_Precache(CModelInstance& modelInstance);

#endif