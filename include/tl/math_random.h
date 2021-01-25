#pragma once
#include "random.h"
#include "math.h"
#include "bits.h"

namespace TL {

template <class State>
inline v3f next_v3f(State &state) {
	return {
		next_f32(state),
		next_f32(state),
		next_f32(state),
	};
}

forceinline u32 random_u32(u32 seed) {
	return seed + reverse_bits(seed) + reverse_bytes(seed);
}
forceinline u32 random_u32(s32 seed) { return random_u32((u32)seed); }
forceinline s32 random_s32(u32 seed) { return (s32)random_u32(seed); }
forceinline s32 random_s32(s32 seed) { return (s32)random_u32(seed); }
forceinline f32 random_f32(u32 seed) { return to_f32_01(random_u32(seed)); }
forceinline f32 random_f32(s32 seed) { return to_f32_01(random_u32(seed)); }

forceinline f32 random_f32(v2f value) {
	v2f a = frac(value * v2f{sqrt2, sqrt3} * 123.45);
	a += dot(a, a + 12.34);
	return frac(a.x * a.y * 136.51f);
}
forceinline f32 random_f32(v3f value) {
	v3f a = frac(value * v3f{sqrt2, sqrt3, sqrt5} * 123.45);
	a += dot(a, a + 12.34);
	a = frac(a * a.yzx() * 136.51f);
	return frac(dot(a, a));
}
forceinline f32x4 random_f32x4(v3fx4 value) {
	v3fx4 a = frac(value * V3fx4(sqrt2, sqrt3, sqrt5) * 123.45);
	a += dot(a, a + 12.34);
	a = frac(a * a.yzx() * 136.51f);
	return frac(dot(a, a));
}
forceinline f32x8 random_f32x8(v3fx8 value) {
	v3fx8 a = frac(value * V3fx8(sqrt2, sqrt3, sqrt5) * 123.45);
	a += dot(a, a + 12.34);
	a = frac(a * a.yzx() * 136.51f);
	return frac(dot(a, a));
}

forceinline v2f random_v2f(v2f value) {
	v2f a = frac(value * v2f{sqrt2, sqrt3} * 123.45);
	return frac(a + dot(a, a + 12.34));
}
forceinline v3f random_v3f(v3f value) {
	v3f a = frac(value * v3f{sqrt2, sqrt3, sqrt5} * 123.45);
	a = frac(a);
	return frac(a * a.yzx() + dot(a, a + 12.34));
}
forceinline v3fx8 random_v3fx8(v3fx8 value) {
	v3fx8 a = frac(value * V3fx8(sqrt2, sqrt3, sqrt5) * 123.45);
	a = frac(a);
	return frac(a * a.yzx() + dot(a, a + 12.34));
}

forceinline v2f random_v2f(v2s value) {
	return {
		random_f32((value.x + reverse_bits(value.y)) ^ 0xABCDEF01), 
		random_f32((value.y - reverse_bits(value.x)) ^ 0x12435687)
	};
}

static constexpr f32 voronoi_largest_distance_possible_2d = 1.5811388300841896659994467722164f; // sqrt(2.5)
static constexpr f32 voronoi_largest_distance_possible_3d = 1.6583123951776999245574663683353f; // sqrt(2.75)
static constexpr f32 voronoi_inv_largest_distance_possible_2d = 1.0f / voronoi_largest_distance_possible_2d;
static constexpr f32 voronoi_inv_largest_distance_possible_3d = 1.0f / voronoi_largest_distance_possible_3d;

forceinline f32 voronoi(v2f coordinate) {
	v2f tile_position = floor(coordinate);
	v2f local_position = coordinate - tile_position;
	f32 min_distance_squared = 1000;

	static constexpr v2f offsets[] = {
		{-1,-1},
		{-1, 0},
		{-1, 1},
		{ 0,-1},
		{ 0, 0},
		{ 0, 1},
		{ 1,-1},
		{ 1, 0},
		{ 1, 1},
	};

	for (auto offset : offsets) {
		min_distance_squared = min(min_distance_squared, distance_squared(local_position, random_v2f(tile_position + offset) + offset));
	}
	
	return sqrt(min_distance_squared) * voronoi_inv_largest_distance_possible_2d;
}
forceinline f32 voronoi(v3f coordinate) {
	v3f tile_position  = floor(coordinate);
	v3f local_position = coordinate - tile_position;

	v3fx8 tile_position_x8  = V3fx8(tile_position);
	v3fx8 local_position_x8 = V3fx8(local_position);

	f32   min_distance_squared    = 1000;
	f32x8 min_distance_squared_x8 = F32x8(min_distance_squared);

	static constexpr v3fx8 offsets_x8[] = {
		{
			-1,-1,-1,-1,-1,-1,-1,-1,
			-1,-1,-1, 0, 0, 0, 1, 1,
			-1, 0, 1,-1, 0, 1,-1, 0,
		},
		{
			-1, 0, 0, 0, 0, 0, 0, 0,
			 1,-1,-1,-1, 0, 0, 0, 1,
			 1,-1, 0, 1,-1, 0, 1,-1,
		},
		{
			0, 0, 1, 1, 1, 1, 1, 1,
			1, 1,-1,-1,-1, 0, 0, 0,
			0, 1,-1, 0, 1,-1, 0, 1,
		}
	};
	static constexpr v3f offsets_x1[] = {
		{ 1, 1,-1},
		{ 1, 1, 0},
		{ 1, 1, 1},
	};
	
	for (auto offset_x8 : offsets_x8) {
		min_distance_squared_x8 = min(min_distance_squared_x8, distance_squared(local_position_x8, random_v3fx8(tile_position_x8 + offset_x8) + offset_x8));
	}
	for (auto offset : offsets_x1) {
		min_distance_squared = min(min_distance_squared, distance_squared(local_position, random_v3f(tile_position + offset) + offset));
	}

	min_distance_squared = min(min_distance_squared, min(min_distance_squared_x8));

	return sqrt(min_distance_squared) * voronoi_largest_distance_possible_3d;
}
forceinline f32 voronoi(v2s coordinate, s32 step_size) {
	v2s scaled_tile = floor(coordinate, step_size);
	v2s tile_position = scaled_tile / step_size;
	v2f local_position = (v2f)(coordinate - scaled_tile) / step_size;
	f32 min_distance_squared = 1000;

	static constexpr v2s offsets[] = {
		{-1,-1},
		{-1, 0},
		{-1, 1},
		{ 0,-1},
		{ 0, 0},
		{ 0, 1},
		{ 1,-1},
		{ 1, 0},
		{ 1, 1},
	};

	for (auto offset : offsets) {
		min_distance_squared = min(min_distance_squared, distance_squared(local_position, random_v2f(tile_position + offset) + (v2f)offset));
	}
	
	return sqrt(min_distance_squared) * voronoi_inv_largest_distance_possible_2d;
}

forceinline f32 noise(v2f coordinate, f32(*interpolate)(f32) = [](f32 v){return v;}) {
	v2f tile_position = floor(coordinate);
	v2f local_position = coordinate - tile_position;

	f32 top_left     = random_f32(tile_position + v2f{0,0});
	f32 bottom_left  = random_f32(tile_position + v2f{0,1});
	f32 bottom_right = random_f32(tile_position + v2f{1,1});
	f32 top_right    = random_f32(tile_position + v2f{1,0});

	f32 tx = interpolate(local_position.x);
	f32 ty = interpolate(local_position.y);
	f32 top    = lerp(   top_left,    top_right, tx);
	f32 bottom = lerp(bottom_left, bottom_right, tx);

	return lerp(top, bottom, ty);
}

forceinline f32 noise(v3f coordinate, f32(*interpolate)(f32) = [](f32 v){return v;}) {
	v3f tile_position = floor(coordinate);
	v3f local_position = coordinate - tile_position;
	
	f32x8 arr = random_f32x8(V3fx8(tile_position) + V3fx8(
		0,0,0,
		0,0,1,
		0,1,0,
		0,1,1,
		1,0,0,
		1,0,1,
		1,1,0,
		1,1,1
	));
	f32 left_bottom_front  = arr.s[0];
	f32 left_bottom_back   = arr.s[1];
	f32 left_top_front     = arr.s[2];
	f32 left_top_back      = arr.s[3];
	f32 right_bottom_front = arr.s[4];
	f32 right_bottom_back  = arr.s[5];
	f32 right_top_front    = arr.s[6];
	f32 right_top_back     = arr.s[7];
	
	f32 tx = interpolate(local_position.x);
	f32 ty = interpolate(local_position.y);
	f32 tz = interpolate(local_position.z);

	f32 bottom_front = lerp(left_bottom_front, right_bottom_front, tx);
	f32 bottom_back = lerp(left_bottom_back, right_bottom_back, tx);
	f32 top_front = lerp(left_top_front, right_top_front, tx);
	f32 top_back = lerp(left_top_back, right_top_back, tx);

	f32 front = lerp(bottom_front, top_front, ty);
	f32 back = lerp(bottom_back, top_back, ty);

	return lerp(front, back, tz);
}

template <class T, class Fn, class ...Args>
forceinline auto layer(u32 count, f32 power, f32 scale, T coordinate, Fn &&fn, Args &&...args) {
	using ResultType = decltype(fn(coordinate, std::forward<Args>(args)...));
	ResultType result = {};
	f32 total_power = 0;
	f32 current_scale = 1;
	f32 current_power = 1;
	for (u32 i = 0; i < count; ++i) {
		result += fn(coordinate * current_scale, std::forward<Args>(args)...) * current_power;
		total_power += current_power;
		current_power *= power;
		current_scale *= scale;
	}
	return result * (1.0f / total_power);
}

template <class T>
struct Smoothed {
	T base = {};
	T target = {};
	T interpolated = {};
	f32 factor = 0;

	forceinline Smoothed() = default;
	forceinline Smoothed(T value) : base(value), target(value), interpolated(value) {}
};

template <class T>
forceinline void update_base(Smoothed<T> &smoothed) {
	smoothed.base = smoothed.interpolated;
	smoothed.factor = 0;
}

template <class T>
forceinline void interpolate(Smoothed<T> &smoothed, f32 delta) {
	smoothed.factor = clamp(smoothed.factor + delta, 0, 1);
	smoothed.interpolated = lerp(smoothed.base, smoothed.target, smoothed.factor);
}

}
