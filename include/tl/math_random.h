#pragma once
#include "random.h"
#include "math.h"
#include "bits.h"

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
	seed ^= 0x55555555u;
	seed *= u32_random_primes[0];
	seed ^= 0x33333333u;
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

forceinline f32 random_f32(v2f value) {
	v2f a = frac(value * v2f{sqrt2, sqrt3} * 123.45f);
	a += dot(a, a + 12.34f);
	return frac(a.x * a.y * 136.51f);
}
forceinline f32 random_f32(v3f value) {
	v3f a = frac(value * v3f{sqrt2, sqrt3, sqrt5} * 123.45f);
	a += dot(a, a + 12.34f);
	a = frac(a * a.yzx() * 136.51f);
	return frac(dot(a, a));
}
forceinline v2f random_v2f(v2f value) {
	v2f a = frac(value * v2f{sqrt2, sqrt3} * 123.45f);
	return frac(a + dot(a, a + 12.34f));
}
forceinline v3f random_v3f(v3f value) {
	v3f a = frac(value * v3f{sqrt2, sqrt3, sqrt5} * 123.45f);
	a = frac(a);
	return frac(a * a.yzx() + dot(a, a + 12.34f));
}

forceinline v2f random_v2f(v2s value) {
	return {
		random_f32((value.x + reverse_bits(value.y)) ^ 0xABCDEF01), 
		random_f32((value.y - reverse_bits(value.x)) ^ 0x12435687)
	};
}

forceinline v3f random_v3f(v3u value) {
	value.x = random_u32(value.x);
	value.y = random_u32(value.x ^ value.y);
	value.z = random_u32(value.y ^ value.z);
	return normalize_range_f32<v3f>(value);
}
forceinline v3f random_v3f(v3s value) { return random_v3f((v3u)value); }

forceinline f32 random_f32(v2u seed) {
	u32 s = 0;
	s ^= random_u32(seed.x);
	s ^= random_u32(seed.y + s);
	return normalize_range_f32<f32>(s);
}
forceinline f32 random_f32(v3u seed) {
	u32 s = 0;
	s ^= random_u32(seed.x);
	s ^= random_u32(seed.y + s);
	s ^= random_u32(seed.z + s);
	return normalize_range_f32<f32>(s);
}
forceinline f32 random_f32(v2s seed) { return random_f32((v2u)seed); }
forceinline f32 random_f32(v3s seed) { return random_f32((v3u)seed); }

forceinline u32 random_u32(f32 seed) { return *(u32 *)&seed; }
forceinline u32 random_u32(v2f seed) { return random_u32(seed.x) ^ random_u32(seed.y); }

forceinline u32 random_u32(v2s seed) { return random_u32(seed.x) ^ random_u32(seed.y); }
forceinline u32 random_u32(v3s seed) { return random_u32(seed.x) ^ random_u32(seed.y) ^ random_u32(seed.z); }

static constexpr f32 voronoi_largest_distance_possible_2d = 1.5811388300841896659994467722164f; // sqrt(2.5)
static constexpr f32 voronoi_largest_distance_possible_3d = 1.6583123951776999245574663683353f; // sqrt(2.75)
static constexpr f32 voronoi_inv_largest_distance_possible_2d = 1.0f / voronoi_largest_distance_possible_2d;
static constexpr f32 voronoi_inv_largest_distance_possible_3d = 1.0f / voronoi_largest_distance_possible_3d;

forceinline f32 voronoi_v2f(v2f coordinate) {
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
forceinline f32 voronoi_v3f(v3f coordinate) {
	v3f tile_position  = floor(coordinate);
	v3f local_position = coordinate - tile_position;

	f32 min_distance_squared = 1000;

	static constexpr v3f offsets[] = {
		{-1,-1,-1}, {-1,-1, 0}, {-1,-1, 1},
		{-1, 0,-1}, {-1, 0, 0}, {-1, 0, 1},
		{-1, 1,-1}, {-1, 1, 0}, {-1, 1, 1},
		{ 0,-1,-1}, { 0,-1, 0}, { 0,-1, 1},
		{ 0, 0,-1}, { 0, 0, 0}, { 0, 0, 1},
		{ 0, 1,-1}, { 0, 1, 0}, { 0, 1, 1},
		{ 1,-1,-1}, { 1,-1, 0}, { 1,-1, 1},
		{ 1, 0,-1}, { 1, 0, 0}, { 1, 0, 1},
		{ 1, 1,-1}, { 1, 1, 0}, { 1, 1, 1},
	};
	
	for (auto offset : offsets) {
		min_distance_squared = min(min_distance_squared, distance_squared(local_position, random_v3f(tile_position + offset) + offset));
	}


	return sqrt(min_distance_squared) * voronoi_largest_distance_possible_3d;
}
forceinline f32 voronoi_v2s(v2s coordinate, s32 step_size) {
	v2s scaled_tile = floor(coordinate, step_size);
	v2s tile_position = scaled_tile / step_size;
	v2f local_position = (v2f)(coordinate - scaled_tile) * reciprocal((f32)step_size);
	f32 min_distance_squared = 1000;

	static constexpr v2s offsets[] = {
		{-1,-1}, {-1, 0}, {-1, 1},
		{ 0,-1}, { 0, 0}, { 0, 1},
		{ 1,-1}, { 1, 0}, { 1, 1},
	};

	for (auto offset : offsets) {
		min_distance_squared = min(min_distance_squared, distance_squared(local_position, random_v2f(tile_position + offset) + (v2f)offset));
	}
	
	return sqrt(min_distance_squared) * voronoi_inv_largest_distance_possible_2d;
}
forceinline f32 voronoi_v3s(v3s coordinate, s32 step_size) {
	v3s scaled_tile = floor(coordinate, step_size);
	v3s tile_position = scaled_tile / step_size;
	v3f local_position = (v3f)(coordinate - scaled_tile) * reciprocal((f32)step_size);
	
	f32 min_distance_squared = 1000;

	static constexpr v3s offsets[] = {
		{-1,-1,-1}, {-1,-1, 0}, {-1,-1, 1},
		{-1, 0,-1}, {-1, 0, 0}, {-1, 0, 1},
		{-1, 1,-1}, {-1, 1, 0}, {-1, 1, 1},
		{ 0,-1,-1}, { 0,-1, 0}, { 0,-1, 1},
		{ 0, 0,-1}, { 0, 0, 0}, { 0, 0, 1},
		{ 0, 1,-1}, { 0, 1, 0}, { 0, 1, 1},
		{ 1,-1,-1}, { 1,-1, 0}, { 1,-1, 1},
		{ 1, 0,-1}, { 1, 0, 0}, { 1, 0, 1},
		{ 1, 1,-1}, { 1, 1, 0}, { 1, 1, 1},
	};
	for (auto offset : offsets) {
		min_distance_squared = min(min_distance_squared, distance_squared(local_position, random_v3f(tile_position + offset) + (v3f)offset));
	}

	return sqrt(min_distance_squared) * voronoi_inv_largest_distance_possible_3d;
}

forceinline f32 value_noise_v2f(v2f coordinate, f32(*interpolate)(f32) = [](f32 v){return v;}) {
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

forceinline f32 value_noise_v3f(v3f coordinate, f32(*interpolate)(f32) = [](f32 v){return v;}) {
	v3f tile_position = floor(coordinate);
	v3f local_position = coordinate - tile_position;
	
	f32 left_bottom_front  = random_f32(tile_position + v3f{0,0,0});
	f32 left_bottom_back   = random_f32(tile_position + v3f{0,0,1});
	f32 left_top_front     = random_f32(tile_position + v3f{0,1,0});
	f32 left_top_back      = random_f32(tile_position + v3f{0,1,1});
	f32 right_bottom_front = random_f32(tile_position + v3f{1,0,0});
	f32 right_bottom_back  = random_f32(tile_position + v3f{1,0,1});
	f32 right_top_front    = random_f32(tile_position + v3f{1,1,0});
	f32 right_top_back     = random_f32(tile_position + v3f{1,1,1});
	
	f32 tx = interpolate(local_position.x);
	f32 ty = interpolate(local_position.y);
	f32 tz = interpolate(local_position.z);

	f32 bottom_front = lerp(left_bottom_front, right_bottom_front, tx);
	f32 bottom_back  = lerp(left_bottom_back,  right_bottom_back,  tx);
	f32 top_front    = lerp(left_top_front,    right_top_front,    tx);
	f32 top_back     = lerp(left_top_back,     right_top_back,     tx);

	f32 front = lerp(bottom_front, top_front, ty);
	f32 back  = lerp(bottom_back,  top_back,  ty);

	return lerp(front, back, tz);
}

forceinline f32 value_noise_v2s(v2s coordinate, s32 step, f32(*interpolate)(f32)) {
	v2s floored = floor(coordinate, step);
    v2s tile = floored / step;
	v2f local = (v2f)(coordinate - floored) * reciprocal((f32)step);

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
template <class Interpolate>
forceinline f32 value_noise_v3s(v3s coordinate, s32 step, Interpolate &&interpolate) {
	static_assert(isSame<decltype(interpolate(0.0f)), f32>);
	v3s floored = floor(coordinate, step);
    v3s tile = floored / step;
	v3f local = (v3f)(coordinate - floored) * reciprocal((f32)step);
	
	f32 tx = interpolate(local.x);
	f32 ty = interpolate(local.y);
	f32 tz = interpolate(local.z);
#if ARCH_AVX2
	__m256i px = _mm256_add_epi32(_mm256_set1_epi32(tile.x), _mm256_setr_epi32(0, 1, 0, 1, 0, 1, 0, 1));
	__m256i py = _mm256_add_epi32(_mm256_set1_epi32(tile.y), _mm256_setr_epi32(0, 0, 1, 1, 0, 0, 1, 1));
	__m256i pz = _mm256_add_epi32(_mm256_set1_epi32(tile.z), _mm256_setr_epi32(0, 0, 0, 0, 1, 1, 1, 1));
	
	auto random_u32x8 = [] (__m256i s) {
		s = _mm256_xor_si256  (s, _mm256_set1_epi32(0x55555555u));
		s = _mm256_mullo_epi32(s, _mm256_set1_epi32(u32_random_primes[0]));
		s = _mm256_xor_si256  (s, _mm256_set1_epi32(0x33333333u));
		s = _mm256_mullo_epi32(s, _mm256_set1_epi32(u32_random_primes[1]));
		return s;
	};

	__m256i s = random_u32x8(px);
	s = _mm256_xor_si256(s, random_u32x8(_mm256_add_epi32(py, s)));
	s = _mm256_xor_si256(s, random_u32x8(_mm256_add_epi32(pz, s)));

	__m256 samples = _mm256_mul_ps(_mm256_cvtepi32_ps(_mm256_srli_epi32(s, 8)), _mm256_set1_ps(1.0f / ((1 << 24) - 1)));

	__m128 a = _mm256_extractf128_ps(samples, 0);
	__m128 b = _mm256_extractf128_ps(samples, 1);

	a = _mm_fmadd_ps(_mm_sub_ps(b, a), _mm_set1_ps(tz), a);

	b = _mm_shuffle_ps(a, a, _MM_SHUFFLE(0, 0, 3, 2));
	a = _mm_fmadd_ps(_mm_sub_ps(b, a), _mm_set1_ps(ty), a);

	b = _mm_shuffle_ps(a, a, _MM_SHUFFLE(0, 0, 0, 1));
	a = _mm_fmadd_ps(_mm_sub_ps(b, a), _mm_set1_ps(tx), a);
	
	return _mm_cvtss_f32(a);
#else
	f32 left_bottom_back   = random_f32(tile + v3s{0,0,0});
	f32 right_bottom_back  = random_f32(tile + v3s{1,0,0});
	f32 left_top_back      = random_f32(tile + v3s{0,1,0});
	f32 right_top_back     = random_f32(tile + v3s{1,1,0});
	f32 left_bottom_front  = random_f32(tile + v3s{0,0,1});
	f32 right_bottom_front = random_f32(tile + v3s{1,0,1});
	f32 left_top_front     = random_f32(tile + v3s{0,1,1});
	f32 right_top_front    = random_f32(tile + v3s{1,1,1});

	f32 left_bottom  = lerp(left_bottom_back,  left_bottom_front,  tz);
	f32 right_bottom = lerp(right_bottom_back, right_bottom_front, tz);
	f32 left_top     = lerp(left_top_back,     left_top_front,     tz);
	f32 right_top    = lerp(right_top_back,    right_top_front,    tz);

	f32 left  = lerp(left_bottom,  left_top,  ty);
	f32 right = lerp(right_bottom, right_top, ty);

	return lerp(left, right, tx);
#endif
}
forceinline f32 value_noise_v2s_linear(v2s coordinate, s32 step) { return value_noise_v2s(coordinate, step, [](f32 v){ return v; }); }
forceinline f32 value_noise_v3s_linear(v3s coordinate, s32 step) { return value_noise_v3s(coordinate, step, [](f32 v){ return v; }); }

forceinline f32 gradient_noise_v2f(v2f coordinate) {
    v2f tile = floor(coordinate);
    v2f local = frac(coordinate);
    
    v2f t0 = local;
    v2f t1 = t0 - 1.;
    
	static const v2f directions[] = {
		v2f{ 1, 0},
		v2f{-1, 0},
		v2f{ 0, 1},
		v2f{ 0,-1},
		normalize(v2f{ 1, 1}),
		normalize(v2f{-1, 1}),
		normalize(v2f{ 1,-1}),
		normalize(v2f{-1,-1})
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

forceinline f32 gradient_noise_v2s(v2s coordinate, s32 step) {
	v2s floored = floor(coordinate, step);
    v2s tile = floored / step;
    v2f local = (v2f)(coordinate - floored) * reciprocal((f32)step);
    
    v2f t0 = local;
    v2f t1 = t0 - 1.;
    
	static const v2f directions[] = {
		v2f{ 1, 0},
		v2f{-1, 0},
		v2f{ 0, 1},
		v2f{ 0,-1},
		normalize(v2f{ 1, 1}),
		normalize(v2f{-1, 1}),
		normalize(v2f{ 1,-1}),
		normalize(v2f{-1,-1})
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

forceinline f32 gradient_noise_v3s(v3s coordinate, s32 step) {
	v3s floored = floor(coordinate, step);
    v3s tile = floored / step;
    v3f local = (v3f)(coordinate - floored) * reciprocal((f32)step);
    
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
