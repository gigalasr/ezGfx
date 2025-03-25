#pragma once
#include <string>

namespace ez {
    const std::string GL_QUAD_VERTEX_SHADER = R"(
		#version 450 core

		struct InstanceData {
			float trans[10];
			ivec2 C;
		};

		layout(std430, binding = 0) buffer TransformSSBO
		{
			InstanceData sData[];
		};
	
		uniform mat4	uProj;
		uniform mat4	uView;
		uniform float	uOffset;
		out		vec3	vTexCoords;

		const float Quad[24] = {
			0.0f, 1.0f, 0.0f, 1.0f,
			1.0f, 0.0f, 1.0f, 0.0f,
			0.0f, 0.0f, 0.0f, 0.0f,

			0.0f, 1.0f, 0.0f, 1.0f,
			1.0f, 1.0f, 1.0f, 1.0f,
			1.0f, 0.0f, 1.0f, 0.0f
		};


		vec4 quat_from_axis_angle(float x, float y, float z, float angle)
		{ 
  			vec4 q;
  			float half_angle = angle * 0.5;
  			q.x = x * sin(half_angle);
  			q.y = y * sin(half_angle);
  			q.z = z * sin(half_angle);
  			q.w = cos(half_angle);
  			return q;
		}

		vec3 rotate_vertex_by_quat(vec3 v, vec4 q)
		{ 
  			return v + 2.0 * cross(q.xyz, cross(q.xyz, v) + q.w * v);
		}

		void main() {
			InstanceData data = sData[gl_VertexID / 6];
			int baseVertex = (gl_VertexID % 6) * 4;

			vec3 pos = vec3(Quad[baseVertex], Quad[baseVertex+1], 0.0f);
			vTexCoords = vec3(Quad[baseVertex+2], Quad[baseVertex+3], data.C.x);

			mat4 model;
			model[0] = vec4(data.trans[3], 0.0, 0.0, 0.0);
			model[1] = vec4(0.0, data.trans[4], 0.0, 0.0);
			model[2] = vec4(0.0, 0.0, 1.0, 0.0);
			model[3] = vec4(data.trans[0], data.trans[1], data.trans[2] + uOffset, 1.0);

			vec4 quat = quat_from_axis_angle(data.trans[5], data.trans[6], data.trans[7], data.trans[8]);

			// scale
			pos *= vec3(data.trans[3], data.trans[4], 1);
			pos -= vec3(data.trans[3] / 2.0, data.trans[4] / 2.0,0);
			pos = rotate_vertex_by_quat(pos, quat);
			pos += vec3(data.trans[3] / 2.0, data.trans[4] / 2.0,0);

			pos += vec3(data.trans[0], data.trans[1], data.trans[2] + uOffset);

			gl_Position = uProj * uView * vec4(pos, 1);
		}
	)";

    const std::string GL_QUAD_FRAGMENT_SHADER = R"(
		#version 450 core

		in		vec3			vTexCoords;
		out		vec4			FragColor;
		uniform	sampler2DArray	uBrushTex;

		// This approximates the error function, needed for the gaussian integral
		vec4 erf(vec4 x) {
		  vec4 s = sign(x), a = abs(x);
		  x = 1.0 + (0.278393 + (0.230389 + 0.078108 * (a * a)) * a) * a;
		  x *= x;
		  return s - s / (x * x);
		}
		
		// Return the mask for the shadow of a box from lower to upper
		float boxShadow(vec2 lower, vec2 upper, vec2 point, float sigma) {
		  vec4 query = vec4(point - lower, upper - point);
		  vec4 integral = 0.5 + 0.5 * erf(query * (sqrt(0.5) / sigma));
		  return (integral.z - integral.x) * (integral.w - integral.y);
		}

		void main() {
			FragColor = texture(uBrushTex, vTexCoords);
			//FragColor *= boxShadow(vec2(0.2, 0.2), vec2(0.8, 0.8), vTexCoords.xy, 0.75);
		}
	)";
}  // namespace ez