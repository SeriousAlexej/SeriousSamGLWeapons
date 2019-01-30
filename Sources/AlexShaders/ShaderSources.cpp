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
#include "ShaderSources.h"

extern const char* g_vertexShader = "#version 330\n\
    layout(location = 0) in vec3 posModel;\n\
    layout(location = 1) in vec2 vertexUV;\n\
    layout(location = 2) in vec3 normalModel;\n\
    layout(location = 3) in vec3 tangentModel;\n\
    layout(location = 4) in vec3 bitangentModel;\n\
    out vec2 UV;\n\
    out vec3 L;\n\
    out vec3 normal;\n\
    out mat3 TBN;\n\
    out vec3 refl;\n\
    out mat4 viewMatrix;\n\
    out vec3 viewDir;\n\
    uniform mat4 projMx;\n\
    uniform mat4 viewMx;\n\
    uniform mat4 modelMx;\n\
    uniform vec3 lightDir;\n\
    void main()\n\
    {\n\
      gl_Position = projMx * viewMx * vec4(posModel, 1.0);\n\
      L = normalize((inverse(viewMx) * vec4(lightDir, 0.0)).xyz);\n\
      UV = vertexUV;\n\
      \n\
      \n\
      normal = (modelMx * vec4(normalModel, 0.0)).xyz;\n\
      vec3 t = normalize((modelMx * vec4(tangentModel, 0.0)).xyz);\n\
      vec3 b = normalize((modelMx * vec4(bitangentModel, 0.0)).xyz);\n\
      vec3 n = normalize((modelMx * vec4(normalModel, 0.0)).xyz);\n\
      TBN = mat3(t, b, n);\n\
      \n\
      \n\
      refl = normalize(vec3(viewMx * vec4(posModel, 1.0)));\n\
      viewMatrix = viewMx;\n\
      viewDir = normalize((inverse(viewMx) * vec4(0.0, 0.0, -1.0, 0.0)).xyz);\n\
    }";

extern const char* g_pixelShader = "#version 330\n\
    out vec4 color;\n\
    in vec2 UV;\n\
    in vec3 L;\n\
    in vec3 normal;\n\
    in mat3 TBN;\n\
    in vec3 refl;\n\
    in mat4 viewMatrix;\n\
    in vec3 viewDir;\n\
    uniform vec3 colLight;\n\
    uniform vec3 colAmbient;\n\
    uniform float normalStrength;\n\
    uniform float reflectionStrength;\n\
    uniform float specularStrength;\n\
    uniform float specularFactor;\n\
    uniform float detailNormalTileU;\n\
    uniform float detailNormalTileV;\n\
    uniform float detailNormalStrength;\n\
    uniform sampler2D diffuseTexture;\n\
    uniform sampler2D normalTexture;\n\
    uniform sampler2D reflectionTexture;\n\
    uniform sampler2D detailNormalTexture;\n\
    uniform sampler2D glossTexture;\n\
  \n\
  void main()\n\
  {\n\
      vec3 colND = texture(detailNormalTexture, vec2(UV.x * detailNormalTileU, UV.y * detailNormalTileV)).xyz;\n\
      vec3 colN = texture(normalTexture, UV).xyz;\n\
      colND = mix(vec3(0.0, 0.0, 1.0), colND * 2.0 - vec3(1.0), detailNormalStrength);\n\
      colN = colN * 2.0 - vec3(1.0);\n\
      vec2 pd = colN.xy / colN.z + colND.xy / colND.z;\n\
      vec3 n = TBN * (1.9921875 * (normalize(vec3(pd, 1.0)) * 0.5 + vec3(0.5)) - 1.0);\n\
      n = normalize((1.0 + normalStrength) * normal - n * normalStrength);\n\
      \n\
      \n\
      vec3 reflPos = reflect(refl, vec3(viewMatrix * vec4(n, 0.0)));\n\
      float m = 2.0 * sqrt(pow(reflPos.x, 2.0) + pow(reflPos.y, 2.0) + pow(reflPos.z + 1.0, 2.0));\n\
      vec2 reflUV = reflPos.xy / m + 0.5;\n\
      float dirCos = clamp(dot(n, L), 0.0, 1.0);\n\
      vec3 reflectionColor = reflectionStrength * texture2D(reflectionTexture, reflUV).rgb;\n\
      reflectionColor = 0.3*colAmbient*reflectionColor + 0.7*reflectionColor*dirCos;\n\
      \n\
      \n\
      vec3 specularReflection = colLight * specularStrength * pow(max(0.0, dot(reflect(L, n), viewDir)), specularFactor);\n\
      specularReflection = texture(glossTexture, UV).x * specularReflection;\n\
      \n\
      \n\
      vec3 colDiffuse = texture(diffuseTexture, UV).rgb;\n\
      color = vec4(reflectionColor + colDiffuse*(colAmbient + dirCos*(colLight + specularReflection)), 1.0);\n\
    }";