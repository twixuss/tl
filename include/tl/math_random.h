#pragma once
#include "random.h"
#include "math.h"
#include "bits.h"
#include "simd/bits.h"

#pragma warning(push)
//#pragma warning(disable : 4244)

namespace TL {

inline static constexpr u32 u32_random_primes[] = {
	0x36DCD91D,
	0x054A136F,
	0x7DA1D8CD,
	0x914907D1,
	0xAF446B4D,
	0xC9B813BD,
	0x89617FB5,
	0x19744049,
};

template <class State>
inline v3f next_v3f(State &state) {
	return {
		next_f32(state),
		next_f32(state),
		next_f32(state),
	};
}

forceinline u32 random_u32(u32 seed) { 
	seed ^= (u32)0x55555555;
	seed *= u32_random_primes[0];
	seed ^= (u32)0x33333333;
	seed *= u32_random_primes[1];
    return seed;
}
forceinline u64 random_u64(u64 seed) {
    u64 r0 = seed;
    u64 r1 = rotateLeft(seed, 32);
    u64 r2 = ((seed << 8) & 0xFF00FF00FF00FF00) | ((seed >> 8) & 0x00FF00FF00FF00FF);
    return r2 * r1 * r0 * 0x8F2A63394D;
}

forceinline u32 random_u32(s32 seed) { return random_u32((u32)seed); }
forceinline s32 random_s32(u32 seed) { return (s32)random_u32(seed); }
forceinline s32 random_s32(s32 seed) { return (s32)random_u32(seed); }
forceinline f32 random_f32(u32 seed) { return normalize_range_f32<f32>(random_u32(seed)); }
forceinline f32 random_f32(s32 seed) { return normalize_range_f32<f32>(random_u32(seed)); }

forceinline u64 random_u64(s64 seed) { return random_u64((u64)seed); }
forceinline s64 random_s64(u64 seed) { return (s64)random_u64(seed); }
forceinline s64 random_s64(s64 seed) { return (s64)random_u64(seed); }
forceinline f64 random_f64(u64 seed) { return normalize_range_f64<f64>(random_u64(seed)); }
forceinline f64 random_f64(s64 seed) { return normalize_range_f64<f64>(random_u64(seed)); }

forceinline u32x8 random_u32x8(u32x8 seed) {
	return seed + reverse_bits(seed) + reverse_bytes(seed);
}
forceinline u32x8 random_u32x8(s32x8 seed) { return random_u32x8((u32x8)seed); }
forceinline s32x8 random_s32x8(u32x8 seed) { return (s32x8)random_u32x8(seed); }
forceinline s32x8 random_s32x8(s32x8 seed) { return (s32x8)random_u32x8(seed); }
forceinline f32x8 random_f32x8(u32x8 seed) { return normalize_range_f32<f32x8>(random_u32x8(seed)); }
forceinline f32x8 random_f32x8(s32x8 seed) { return normalize_range_f32<f32x8>(random_u32x8(seed)); }

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

forceinline v3f random_v3f(v3s value) {
	return {
		random_f32((value.x + reverse_bits(value.y)) ^ 0xAFE3DA15), 
		random_f32((value.y + reverse_bits(value.z)) ^ 0x200EE364),
		random_f32((value.z + reverse_bits(value.x)) ^ 0x512621F6),
	};
}
forceinline v3fx8 random_v3fx8(v3sx8 value) {
	return {
		random_f32x8((value.x + reverse_bits(value.y)) ^ 0xAFE3DA15), 
		random_f32x8((value.y + reverse_bits(value.z)) ^ 0x200EE364),
		random_f32x8((value.z + reverse_bits(value.x)) ^ 0x512621F6),
	};
}

forceinline f32 random_f32(v2u seed) {
	u32 s = 0;
	s ^= random_u32(seed.x);
	s ^= random_u32(seed.y + 0x36DCD91D);
	return normalize_range_f32<f32>(s);
}
forceinline f32 random_f32(v3u seed) {
	u32 s = 0;
	s ^= random_u32(seed.x);
	s ^= random_u32(seed.y + 0x55555555);
	s ^= random_u32(seed.z + 0xAAAAAAAA);
	return normalize_range_f32<f32>(s);
}
forceinline f32x8 random_f32x8(v3ux8 seed) {
	u32x8 s = {};
	s ^= random_u32x8(seed.x);
	s ^= random_u32x8(seed.y + 0x55555555);
	s ^= random_u32x8(seed.z + 0xAAAAAAAA);
	return normalize_range_f32<f32x8>(s);
}
forceinline f32 random_f32(v2s seed) { return random_f32((v2u)seed); }
forceinline f32 random_f32(v3s seed) { return random_f32((v3u)seed); }
forceinline f32x8 random_f32x8(v3sx8 seed) { return random_f32x8((v3ux8)seed); }

forceinline u32 random_u32(f32 seed) { return *(u32 *)&seed; }
forceinline u32 random_u32(v2f seed) { return random_u32(seed.x) ^ random_u32(seed.y); }

forceinline u32 random_u32(v2s seed) { return random_u32(seed.x) ^ random_u32(seed.y); }
forceinline u32 random_u32(v3s seed) { return random_u32(seed.x) ^ random_u32(seed.y) ^ random_u32(seed.z); }

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
	v2f local_position = (v2f)(coordinate - scaled_tile) * (1.0f / (f32)step_size);
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
forceinline f32 voronoi(v3s coordinate, s32 step_size) {
	v3s scaled_tile = floor(coordinate, step_size);
	v3s tile_position = scaled_tile / step_size;
	v3f local_position = (v3f)(coordinate - scaled_tile) * (1.0f / (f32)step_size);
	
	v3sx8 tile_position_x8  = V3sx8(tile_position);
	v3fx8 local_position_x8 = V3fx8(local_position);

	f32   min_distance_squared    = 1000;
	f32x8 min_distance_squared_x8 = F32x8(min_distance_squared);

	static constexpr v3sx8 offsets_x8[] = {
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
	static constexpr v3s offsets_x1[] = {
		{ 1, 1,-1},
		{ 1, 1, 0},
		{ 1, 1, 1},
	};
	
	for (auto offset_x8 : offsets_x8) {
		min_distance_squared_x8 = min(min_distance_squared_x8, distance_squared(local_position_x8, random_v3fx8(tile_position_x8 + offset_x8) + (v3fx8)offset_x8));
	}
	for (auto offset : offsets_x1) {
		min_distance_squared = min(min_distance_squared, distance_squared(local_position, random_v3f(tile_position + offset) + (v3f)offset));
	}
	min_distance_squared = min(min_distance_squared, min(min_distance_squared_x8));
	
	return sqrt(min_distance_squared) * voronoi_inv_largest_distance_possible_3d;
}

forceinline f32 value_noise(v2f coordinate, f32(*interpolate)(f32) = [](f32 v){return v;}) {
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

forceinline f32 value_noise(v3f coordinate, f32(*interpolate)(f32) = [](f32 v){return v;}) {
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

forceinline f32 value_noise(v2s coordinate, s32 step, f32(*interpolate)(f32)) {
	v2s floored = floor(coordinate, step);
    v2s tile = floored / step;
	v2f local = (v2f)(coordinate - floored) * (1.0f / (f32)step);

	f32 top_left     = random_f32(tile + v2s{0,0});
	f32 bottom_left  = random_f32(tile + v2s{0,1});
	f32 bottom_right = random_f32(tile + v2s{1,1});
	f32 top_right    = random_f32(tile + v2s{1,0});

	f32 tx = interpolate(local.x);
	f32 ty = interpolate(local.y);
	f32 top    = lerp(   top_left,    top_right, tx);
	f32 bottom = lerp(bottom_left, bottom_right, tx);

	return lerp(top, bottom, ty);
}
forceinline f32 value_noise(v3s coordinate, s32 step, f32(*interpolate)(f32)) {
	v3s floored = floor(coordinate, step);
    v3s tile = floored / step;
	v3f local = (v3f)(coordinate - floored) * (1.0f / (f32)step);
	
	f32 left_top_back      = random_f32(tile + v3s{0,0,0});
	f32 right_top_back     = random_f32(tile + v3s{1,0,0});
	f32 left_bottom_back   = random_f32(tile + v3s{0,1,0});
	f32 right_bottom_back  = random_f32(tile + v3s{1,1,0});
	f32 left_top_front     = random_f32(tile + v3s{0,0,1});
	f32 right_top_front    = random_f32(tile + v3s{1,0,1});
	f32 left_bottom_front  = random_f32(tile + v3s{0,1,1});
	f32 right_bottom_front = random_f32(tile + v3s{1,1,1});

	f32 tx = interpolate(local.x);
	f32 ty = interpolate(local.y);
	f32 tz = interpolate(local.z);
	f32 top_back     = lerp(left_top_back,     right_top_back,     tx);
	f32 bottom_back  = lerp(left_bottom_back,  right_bottom_back,  tx);
	f32 top_front    = lerp(left_top_front,    right_top_front,    tx);
	f32 bottom_front = lerp(left_bottom_front, right_bottom_front, tx);
	f32 top    = lerp(top_back,    top_front,    tz);
	f32 bottom = lerp(bottom_back, bottom_front, tz);
	return lerp(top, bottom, ty);
}
forceinline f32 value_noise(v2s coordinate, s32 step) { return value_noise(coordinate, step, [](f32 v){ return v; }); }
forceinline f32 value_noise(v3s coordinate, s32 step) { return value_noise(coordinate, step, [](f32 v){ return v; }); }

template <class T>
forceinline T saturate(T t) {
	return clamp(t, cvt<T>(0), cvt<T>(1));
}

template <class T>
forceinline T smoothstep(T t) {
	t = saturate(t);
	return t * t * t * (t * (t * 6 - 15) + 10);
}

forceinline f32 gradient_noise(v2f coordinate) {
    v2f tile = floor(coordinate);
    v2f local = frac(coordinate);
    
    v2f t0 = local;
    v2f t1 = t0 - 1.;
    
	static const v2f directions[] = {
		v2f{ 1., 0.},
		v2f{-1., 0.},
		v2f{ 0., 1.},
		v2f{ 0.,-1.},
		normalize(v2f{ 1., 1.}),
		normalize(v2f{-1., 1.}),
		normalize(v2f{ 1.,-1.}),
		normalize(v2f{-1.,-1.})
	};
	static_assert(is_power_of_2(count_of(directions)));
	auto get_direction = [&](v2f offset) { return directions[(random_u32(tile + offset) >> 13) & (count_of(directions) - 1)]; };
    v2f g00 = get_direction(v2f{0, 0});
    v2f g10 = get_direction(v2f{1, 0});
    v2f g01 = get_direction(v2f{0, 1});
    v2f g11 = get_direction(v2f{1, 1});
    
    f32 v00 = g00.x * t0.x + g00.y * t0.y;
    f32 v10 = g10.x * t1.x + g10.y * t0.y;
    f32 v01 = g01.x * t0.x + g01.y * t1.y;
    f32 v11 = g11.x * t1.x + g11.y * t1.y;
    
    v2f t = smoothstep(local);
    return lerp(lerp(v00, v10, t.x), lerp(v01, v11, t.x), t.y) * (sqrt2 / 2.0f) + 0.5f;
}

forceinline f32 gradient_noise(v2s coordinate, s32 step) {
	v2s floored = floor(coordinate, step);
    v2s tile = floored / step;
    v2f local = (v2f)(coordinate - floored) * (1.0f / (f32)step);
    
    v2f t0 = local;
    v2f t1 = t0 - 1.;
    
	static const v2f directions[] = {
		v2f{ 1., 0.},
		v2f{-1., 0.},
		v2f{ 0., 1.},
		v2f{ 0.,-1.},
		normalize(v2f{ 1., 1.}),
		normalize(v2f{-1., 1.}),
		normalize(v2f{ 1.,-1.}),
		normalize(v2f{-1.,-1.})
	};
	static_assert(is_power_of_2(count_of(directions)));
	auto get_direction = [&](v2s offset) { return directions[(random_u32(tile + offset) >> 13) & (count_of(directions) - 1)]; };
    v2f g00 = get_direction(v2s{0, 0});
    v2f g10 = get_direction(v2s{1, 0});
    v2f g01 = get_direction(v2s{0, 1});
    v2f g11 = get_direction(v2s{1, 1});
    
    f32 v00 = g00.x * t0.x + g00.y * t0.y;
    f32 v10 = g10.x * t1.x + g10.y * t0.y;
    f32 v01 = g01.x * t0.x + g01.y * t1.y;
    f32 v11 = g11.x * t1.x + g11.y * t1.y;
    
    v2f t = smoothstep(local);
    return lerp(lerp(v00, v10, t.x), lerp(v01, v11, t.x), t.y) * (sqrt2 / 2.0f) + 0.5f;
}

forceinline f32 gradient_noise(v3s coordinate, s32 step) {
	v3s floored = floor(coordinate, step);
    v3s tile = floored / step;
    v3f local = (v3f)(coordinate - floored) * (1.0f / (f32)step);
    
    v3f t0 = local;
    v3f t1 = t0 - 1;
    
	static constexpr v3f directions[] = {
		// v3f{ 1, 0, 0},
		// v3f{-1, 0, 0},
		// v3f{ 0, 1, 0},
		// v3f{ 0,-1, 0},
		// v3f{ 0, 0, 1},
		// v3f{ 0, 0,-1},
		CE::normalize(v3f{ 1, 1, 1}),
		CE::normalize(v3f{-1, 1, 1}),
		CE::normalize(v3f{ 1,-1, 1}),
		CE::normalize(v3f{-1,-1, 1}),
		CE::normalize(v3f{ 1, 1,-1}),
		CE::normalize(v3f{-1, 1,-1}),
		CE::normalize(v3f{ 1,-1,-1}),
		CE::normalize(v3f{-1,-1,-1})
	};
	static_assert(is_power_of_2(count_of(directions)));

    v3f t = smoothstep(local);

#if ARCH_AVX2
	s32x8 gradient_indices = (s32x8)(random_f32x8(V3sx8(tile) + V3sx8(0, 0, 0,
																	  1, 0, 0,
																	  0, 1, 0,
																	  1, 1, 0,
																	  0, 0, 1,
																	  1, 0, 1,
																	  0, 1, 1,
																	  1, 1, 1)) * count_of(directions)) * sizeof(directions[0]);
	v3fx8 g;
	gather(g.x, &directions->x, gradient_indices);
	gather(g.y, &directions->y, gradient_indices);
	gather(g.z, &directions->z, gradient_indices);

	f32x8 v = dot(g, V3fx8(
		t0.x, t0.y, t0.z,
		t1.x, t0.y, t0.z,
		t0.x, t1.y, t0.z,
		t1.x, t1.y, t0.z,
		t0.x, t0.y, t1.z,
		t1.x, t0.y, t1.z,
		t0.x, t1.y, t1.z,
		t1.x, t1.y, t1.z
	));

    return 
		lerp(
			lerp(lerp(v.s[0], v.s[1], t.x), lerp(v.s[2], v.s[3], t.x), t.y),
			lerp(lerp(v.s[4], v.s[5], t.x), lerp(v.s[6], v.s[7], t.x), t.y),
			t.z
		) / sqrt3 + 0.5f;
#else
	auto get_direction = [&](v3s offset) { return directions[(random_u32(tile + offset) >> 13) & (count_of(directions) - 1)]; };
    v3f g000 = get_direction({0, 0, 0});
    v3f g100 = get_direction({1, 0, 0});
    v3f g010 = get_direction({0, 1, 0});
    v3f g110 = get_direction({1, 1, 0});
    v3f g001 = get_direction({0, 0, 1});
    v3f g101 = get_direction({1, 0, 1});
    v3f g011 = get_direction({0, 1, 1});
    v3f g111 = get_direction({1, 1, 1});
	f32 v000 = dot(g000, v3f{ t0.x, t0.y, t0.z});
    f32 v100 = dot(g100, v3f{ t1.x, t0.y, t0.z});
    f32 v010 = dot(g010, v3f{ t0.x, t1.y, t0.z});
    f32 v110 = dot(g110, v3f{ t1.x, t1.y, t0.z});
	f32 v001 = dot(g001, v3f{ t0.x, t0.y, t1.z});
    f32 v101 = dot(g101, v3f{ t1.x, t0.y, t1.z});
    f32 v011 = dot(g011, v3f{ t0.x, t1.y, t1.z});
    f32 v111 = dot(g111, v3f{ t1.x, t1.y, t1.z});
    
    return 
		lerp(
			lerp(lerp(v000, v100, t.x), lerp(v010, v110, t.x), t.y),
			lerp(lerp(v001, v101, t.x), lerp(v011, v111, t.x), t.y),
			t.z
		) / sqrt3 + 0.5f;
#endif
}


template <class T, class Fn, class ...Args>
forceinline auto layer(u32 count, f32 power, f32 scale, Fn &&fn, T coordinate, Args &&...args) {
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

#pragma warning(pop)
