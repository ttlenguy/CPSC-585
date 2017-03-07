// ==========================================================================
// Vertex program for barebones GLFW boilerplate
//
// Author:  Sonny Chan, University of Calgary
// Date:    December 2015
// ==========================================================================
#version 410


#extension GL_ARB_explicit_uniform_location : enable
#extension GL_ARB_explicit_attrib_location : enable

layout(location = 0) in vec3 VertexPosition;
layout(location = 1) in vec3 VertexNormal;
layout(location = 2) in mat4 Transform;
// 3
// 4
// 5
layout(location = 6) uniform vec3 InstanceColor;
layout(location = 7) uniform vec3 InstanceEmissionColor;

layout(location = 8) uniform mat4 Projection;
layout(location = 9) uniform mat4 View;

layout(location = 10) uniform vec3 LightDirection;
layout(location = 11) uniform vec3 AmbientLight;

layout(location = 12) uniform mat4 shadowMVP;
layout(location = 13) uniform int softShadow;

out vec3 Normal;
out vec4 ShadowCoord;
out vec3 Color;
out vec3 EmissionColor;

void main()
{
    gl_Position = Projection * View * Transform * vec4(VertexPosition, 1.0);
	Normal=mat3(Transform)*VertexNormal;
	ShadowCoord = shadowMVP * Transform * vec4(VertexPosition, 1.0);
	Color=InstanceColor;
	EmissionColor=InstanceEmissionColor;
}
