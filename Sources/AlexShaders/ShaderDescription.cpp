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
#include "Shader.h"

#define TEXTURE_COUNT 5
#define UVMAPS_COUNT  1
#define COLOR_COUNT   0
#define FLOAT_COUNT   7
#define FLAGS_COUNT   0

#define DIFFUSE_TEXTURE       0
#define NORMAL_TEXTURE        1
#define REFLECTION_TEXTURE    2
#define DETAIL_NORMAL_TEXTURE 3
#define GLOSS_MASK_TEXTURE    4
#define UVMAP                 0

extern void Rendah();

SHADER_MAIN(SS3Standard)
{
  shaSetTextureWrapping(GFX_REPEAT, GFX_REPEAT);
  shaEnableDepthTest();
  shaDepthFunc(GFX_LESS_EQUAL);
  shaCullFace(GFX_BACK);
  shaDisableAlphaTest();
  shaDisableBlend();
  shaEnableDepthWrite();
  Rendah();
}

SHADER_DESC(SS3Standard, ShaderDesc& shDesc)
{
  shDesc.sd_astrTextureNames.New(TEXTURE_COUNT);
  shDesc.sd_astrTexCoordNames.New(UVMAPS_COUNT);
  shDesc.sd_astrFloatNames.New(FLOAT_COUNT);

  shDesc.sd_astrTextureNames[DIFFUSE_TEXTURE] = "base texture";
  shDesc.sd_astrTextureNames[NORMAL_TEXTURE] = "normal map";
  shDesc.sd_astrTextureNames[REFLECTION_TEXTURE] = "reflection map";
  shDesc.sd_astrTextureNames[DETAIL_NORMAL_TEXTURE] = "detail normalmap";
  shDesc.sd_astrTextureNames[GLOSS_MASK_TEXTURE] = "gloss mask";
  shDesc.sd_astrTexCoordNames[UVMAP] = "uvmap";
  shDesc.sd_astrFloatNames[0] = "bump strength";
  shDesc.sd_astrFloatNames[1] = "reflection strength";
  shDesc.sd_astrFloatNames[2] = "specularity strength";
  shDesc.sd_astrFloatNames[3] = "specularity factor";
  shDesc.sd_astrFloatNames[4] = "detail stretch U";
  shDesc.sd_astrFloatNames[5] = "detail stretch V";
  shDesc.sd_astrFloatNames[6] = "detail roughness";
  shDesc.sd_strShaderInfo = "SS3 Standard";
}
