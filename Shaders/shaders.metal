#include <metal_stdlib>
#include "constants.h"

using namespace metal;

struct Uniforms {
	float4x4 proj, view, view_normal;
	float2 pixel_scale;
	float screen_scale;
};

struct Lights {
	float4 lights[3];
	uint num_lights;
};

struct ConstantVertexOut {
	float4 clip_pos [[position]];
	float4 color;
};

// Vertex shader for rendering 2D triangles
vertex ConstantVertexOut constant_vs(
	constant float4 *positions  [[buffer(G_BUFFER_POSITIONS)]],
	constant float4 *colors     [[buffer(G_BUFFER_COLORS)]],
	constant Uniforms &uniforms [[buffer(G_BUFFER_MATRICES)]],
	uint vertex_id              [[vertex_id]])
{
	return {
		uniforms.proj * (uniforms.view * float4(positions[vertex_id].xyz, 1.0)),
		colors[vertex_id],
	};
}

// Fragment shader for rendering 2D triangles
fragment float4 constant_fs(ConstantVertexOut fragment_in [[stage_in]])
{
	return fragment_in.color;
}

struct LightingVertexOut {
	float4 clip_pos [[position]];
	float4 color;
	float4 normal;
	float4 view_pos;
};

// Vertex shader for rendering 3D triangles
vertex LightingVertexOut lighting_vs(
	constant float4 *positions  [[buffer(G_BUFFER_POSITIONS)]],
	constant float4 *colors     [[buffer(G_BUFFER_COLORS)]],
	constant float4 *normals    [[buffer(G_BUFFER_NORMALS)]],
	constant Uniforms &uniforms [[buffer(G_BUFFER_MATRICES)]],
	uint vertex_id              [[vertex_id]])
{
	float4 view_pos = uniforms.view * float4(positions[vertex_id].xyz, 1.0);
	return {
		uniforms.proj * view_pos,
		colors[vertex_id],
		uniforms.view_normal * normals[vertex_id],
		view_pos,
	};
}

// Fragment shader for rendering 3D triangles
fragment float4 lighting_fs(
	LightingVertexOut fragment_in [[stage_in]],
	bool              is_front    [[front_facing]],
	constant Lights   &lights     [[buffer(G_BUFFER_LIGHTS)]])
{
	float4 color = fragment_in.color;
	float3 normal = normalize(fragment_in.normal.xyz);
	float3 view_dir = normalize(fragment_in.view_pos.xyz);
	float3 accum = float3(0.0);
	normal *= is_front ? 1.0 : -1.0;

	for (uint i = 0; i < lights.num_lights; i++) {
		float4 light = lights.lights[i];
		float3 direction = light.xyz;
		float power = light.w;
		float3 half_vec = normalize(direction - view_dir);
		float amb_dif = power * saturate(dot(direction, normal));
		float spec = power * powr(saturate(dot(normal, half_vec)), 64); // shininess = 64
		accum += amb_dif * color.rgb + spec;
	}

	return float4(accum, color.a);
}

struct Line {
	float4 pos[2];
};

struct LineVertexOut {
	float4 clip_pos [[position]];
	float4 color;
	float coord;
};

// Vertex shader for rendering lines
vertex LineVertexOut line_vs(
	constant Line *positions    [[buffer(G_BUFFER_POSITIONS)]],
	constant float4 *colors     [[buffer(G_BUFFER_COLORS)]],
	constant Uniforms &uniforms [[buffer(G_BUFFER_MATRICES)]],
	uint vertex_id              [[vertex_id]],
	uint instance_id            [[instance_id]])
{
	constant Line &line = positions[instance_id];
	float4 view[] = {
		uniforms.view * float4(line.pos[0].xyz, 1),
		uniforms.view * float4(line.pos[1].xyz, 1),
	};
	float4 clip[] = {
		uniforms.proj * view[0],
		uniforms.proj * view[1],
	};

	float2 R = view[1].xy / clip[1].w - view[0].xy / clip[0].w;
	float2 r = normalize(float2(-R.y, R.x));
	uint hi = vertex_id >> 1;
	uint lo = vertex_id & 1;
	float size = line.pos[hi].w;
	float2 half_width = size * uniforms.pixel_scale;
	float4 offset = uniforms.proj * float4(half_width * r * clip[hi].w, 0, 0);
	float c = length(R / uniforms.pixel_scale) / 128.0;
	return {
		clip[hi] + (lo ? -1.0 : 1.0) * offset,
		colors[instance_id << 1 | hi],
		(hi ? -1.0 : 1.0) * c
	};
}

// Fragment shader for rendering lines
fragment float4 line_fs(
	LineVertexOut fragment_in [[stage_in]],
	constant uchar &stipple [[buffer(G_BUFFER_STIPPLE)]])
{
	uchar i = uchar(fract(fragment_in.coord) * 8.0);
	uchar a = (stipple >> i) & (uchar)1;
	if (a == (uchar)0) discard_fragment();
	return fragment_in.color;
}

struct MarkerVertexOut {
	float4 clip_pos [[position]];
	float point_size [[point_size]];
	float4 color;
	float4 view_pos;
	float radius;
};

vertex MarkerVertexOut marker_standard_vs(
	constant float4 *positions  [[buffer(G_BUFFER_POSITIONS)]],
	constant float4 *colors     [[buffer(G_BUFFER_COLORS)]],
	constant Uniforms &uniforms [[buffer(G_BUFFER_MATRICES)]],
	uint vertex_id              [[vertex_id]])
{
	float4 view_pos = uniforms.view * float4(positions[vertex_id].xyz, 1.0);
	float4 proj_pos = uniforms.proj * view_pos;
	float size = positions[vertex_id].w;
	return {
		proj_pos,
		size * uniforms.screen_scale,
		colors[vertex_id],
		view_pos,
		size * proj_pos.w * uniforms.pixel_scale.y
	};
}

vertex MarkerVertexOut marker_virtual_vs(
	constant float4 *positions  [[buffer(G_BUFFER_POSITIONS)]],
	constant float4 *colors     [[buffer(G_BUFFER_COLORS)]],
	constant Uniforms &uniforms [[buffer(G_BUFFER_MATRICES)]],
	uint vertex_id              [[vertex_id]])
{
	float4 view_pos = uniforms.view * float4(positions[vertex_id].xyz, 1.0);
	float4 proj_pos = uniforms.proj * view_pos;
	float size = positions[vertex_id].w;
	return {
		proj_pos,
		size * uniforms.screen_scale / (proj_pos.w * uniforms.pixel_scale.y),
		colors[vertex_id],
		view_pos,
		size
	};
}

fragment float4 marker_square_fs(
	MarkerVertexOut fragment_in [[stage_in]])
{
	return fragment_in.color;
}

fragment float4 marker_circle_fs(
	MarkerVertexOut fragment_in [[stage_in]],
	float2          point_coord [[point_coord]])
{
	float2 coord = point_coord * 2.0 - 1.0;
	float discriminant = 1.0 - dot(coord, coord);
	if (discriminant <= 0.0) discard_fragment();
	return fragment_in.color;
}

struct FragmentOutWithDepth {
	float4 color [[color(0)]];
	float depth  [[depth(less)]];
};

fragment FragmentOutWithDepth marker_sphere_fs(
	MarkerVertexOut fragment_in [[stage_in]],
	constant Uniforms &uniforms [[buffer(G_BUFFER_MATRICES)]],
	float2   point_coord        [[point_coord]])
{
	float4 color = fragment_in.color;
	float2 coord = point_coord * 2.0 - 1.0;
	float discriminant = 1.0 - dot(coord, coord);
	if (discriminant <= 0.0) discard_fragment();
	float4 normal(coord, sqrt(discriminant), 0);
	float4 pos = uniforms.proj * (fragment_in.view_pos + normal * fragment_in.radius);
	return {
		float4(color.rgb * normal.z, color.a),
		pos.z / pos.w
	};
}

struct TextVertexIn
{
	float2 pos;
	float2 coord;
};

struct TextVertexOut
{
	float4 clip_pos [[position]];
	float2 coord;
};

vertex TextVertexOut text_vs(
	constant TextVertexIn *vertex_in [[buffer(0)]],
	uint     vertex_id [[vertex_id]])
{
	return {
		float4(vertex_in[vertex_id].pos, 0, 1),
		vertex_in[vertex_id].coord
	};
}

fragment float4 text_fs(
	TextVertexOut fragment_in [[stage_in]],
	texture2d<float, access::read> tex [[texture(0)]])
{
	return tex.read((uint2)fragment_in.coord);
}
