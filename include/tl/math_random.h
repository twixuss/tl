#pragma once
#include "random.h"
#include "math.h"
#include "bits.h"
//#include "generated/simd.h"
//#include "generated/simd_vector.h"

#pragma warning(push)
//#pragma warning(disable : 4244)

namespace tl {

constexpr u32 next_prime_after_2_to_power_of_7_point_5 = 181;
constexpr u32 next_prime_after_2_to_power_of_31_point_5 = 3037000507;

inline constexpr u32 random_primes_u32[] = {
	3282017723,
	351133033,
	508145159,
	2855044457,
	1631354399,
	2632283831,
	2370294643,
	1938940147,
	562213447,
	2126948981,
	3949557629,
	3678122521,
	3655713521,
	1228498429,
	599422001,
	4239407353,
	4093924871,
	3493474037,
	2602211317,
	2449047701,
	626704889,
	13879027,
	156043579,
	745948201,
	3816082019,
	2596961827,
	1038441493,
	2028981233,
	2500564919,
	2881014971,
	2860922219,
	2694697771,
	3346293191,
	1624834027,
	4021269487,
	2326660187,
	3766783021,
	2105476313,
	679589551,
	1501766527,
	1251458261,
	513627721,
	2290561979,
	2584981253,
	2375601649,
	1134211073,
	1941167843,
	942529583,
	516400249,
	1570433611,
	1378307321,
	3092642191,
	698915071,
	925783,
	575859899,
	979311371,
	2967849541,
	707293193,
	4205096429,
	1408547929,
	2001825193,
	1326360691,
	1497718219,
	1175102387,
};

template <class State>
inline v2f next_v2f(State &state) {
	return {
		next_f32(state),
		next_f32(state),
	};
}

template <class State>
inline v3f next_v3f(State &state) {
	return {
		next_f32(state),
		next_f32(state),
		next_f32(state),
	};
}

namespace default_randomizer {

template <umm size_out, umm size_in>
forceinline Array<u8, size_out> random_bytes(Array<u8, size_in> in) = delete; // not implemented

template <class Out, class In>
struct StupidMiddleman {
	forceinline static Out random(In in) requires requires { random_bytes<sizeof(Out)>(std::bit_cast<Array<u8, sizeof(In)>>(in)); } {
		return std::bit_cast<Out>(random_bytes<sizeof(Out)>(std::bit_cast<Array<u8, sizeof(In)>>(in)));
	}
};

template <class In>
struct StupidMiddleman<f32, In> { forceinline static f32 random(In in) {
	return normalize_range_f32<f32>(std::bit_cast<u32>(random_bytes<sizeof(f32)>(std::bit_cast<Array<u8, sizeof(In)>>(in))));
}};
template <class In>
struct StupidMiddleman<v2f, In> { forceinline static v2f random(In in) {
	return normalize_range_f32<v2f>(std::bit_cast<v2u>(random_bytes<sizeof(v2f)>(std::bit_cast<Array<u8, sizeof(In)>>(in))));
}};
template <class In>
struct StupidMiddleman<v3f, In> { forceinline static v3f random(In in) {
	return normalize_range_f32<v3f>(std::bit_cast<v3u>(random_bytes<sizeof(v3f)>(std::bit_cast<Array<u8, sizeof(In)>>(in))));
}};
template <class In>
struct StupidMiddleman<v4f, In> { forceinline static v4f random(In in) {
	return normalize_range_f32<v4f>(std::bit_cast<v4u>(random_bytes<sizeof(v4f)>(std::bit_cast<Array<u8, sizeof(In)>>(in))));
}};


template <class Out, class In>
forceinline Out random(In in) requires requires { StupidMiddleman<Out, In>::random(in); } {
	return StupidMiddleman<Out, In>::random(in);
}

template <>
forceinline Array<u8, 4> random_bytes(Array<u8, 4> in) {
	const u32 k = 3037000507u; // next_prime(2**31.5)
	u32 x = std::bit_cast<u32>(in);
	x = x*k^k;
	x = x*k^k;
	x = x*k^k;
	x = x*k^k;
	return std::bit_cast<Array<u8, 4>>(x);
}

template <>
forceinline Array<u8, 4> random_bytes(Array<u8, 8> in) {
	u64 const k = 11400714819322457659; // next_prime(2**64 / phi)
	u64 x = std::bit_cast<u64>(in);
	x = x*k^k;
	x = x*k^k;

	u32 r = x ^ (x >> 32);
	return std::bit_cast<Array<u8, 4>>(r);
}

// NOT TESTED FOR QUALITY
template <>
forceinline Array<u8, 8> random_bytes(Array<u8, 8> in) {
	u64 const k = 11400714819322457659; // next_prime(2**64 / phi)
	u64 x = std::bit_cast<u64>(in);
	x = x*k^k;
	x = x*k^k;
	x = x*k^k;
	x = x*k^k;
	return std::bit_cast<Array<u8, 8>>(x);
}

template <>
forceinline Array<u8, 12> random_bytes(Array<u8, 4> in) {
	u32 x = std::bit_cast<u32>(in);
	v3u r = {
		random<u32>(x + random_primes_u32[0]),
		random<u32>(x + random_primes_u32[1]),
		random<u32>(x + random_primes_u32[2]),
	};
	return std::bit_cast<Array<u8, 12>>(r);
}

template <>
forceinline Array<u8, 12> random_bytes(Array<u8, 16> in) {
	__m128i x = std::bit_cast<__m128i>(in);
	x = _mm_aesenc_si128(x, _mm_set_epi64x(0x9b68c80c8405f4d9, 0x1bf1b0f0d96f7f3d));
	x = _mm_aesenc_si128(x, _mm_set_epi64x(0x23df27c1dd3eee5e, 0xb5bcbd5448dc1f4c));
	//x = _mm_aesenc_si128(x, _mm_set_epi64x(0xca00b5128b1edd01, 0x038b07db0eed3c04));
	return *(Array<u8, 12> *)&x;
}

}

struct DefaultRandomizer {
	template <class Out, class In>
	forceinline static Out random(In in) requires requires { default_randomizer::random<Out>(in); } {
		return default_randomizer::random<Out>(in);
	}
};

static constexpr f32 voronoi_largest_possible_distance_2d = 1.5811388300841896659994467722164f; // sqrt(2.5)
static constexpr f32 voronoi_largest_possible_distance_3d = 1.6583123951776999245574663683353f; // sqrt(2.75)
static constexpr f32 voronoi_inv_largest_possible_distance_2d = 1.0f / voronoi_largest_possible_distance_2d;
static constexpr f32 voronoi_inv_largest_possible_distance_3d = 1.0f / voronoi_largest_possible_distance_3d;

template <class Randomizer = DefaultRandomizer>
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
		min_distance_squared = min(min_distance_squared, distance_squared(local_position, Randomizer{}.template random<v2f>(tile_position + offset) + offset));
	}

	return sqrt(min_distance_squared) * voronoi_inv_largest_possible_distance_2d;
}
template <class Randomizer = DefaultRandomizer>
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
		min_distance_squared = min(min_distance_squared, distance_squared(local_position, Randomizer{}.template random<v3f>(tile_position + offset) + offset));
	}


	return sqrt(min_distance_squared) * voronoi_largest_possible_distance_3d;
}
template <class Randomizer = DefaultRandomizer>
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
		min_distance_squared = min(min_distance_squared, distance_squared(local_position, Randomizer{}.template random<v2f>(tile_position + offset) + (v2f)offset));
	}

	return sqrt(min_distance_squared) * voronoi_inv_largest_possible_distance_2d;
}
template <class Randomizer = DefaultRandomizer>
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
		min_distance_squared = min(min_distance_squared, distance_squared(local_position, Randomizer{}.template random<v3f>(tile_position + offset) + (v3f)offset));
	}

	return sqrt(min_distance_squared) * voronoi_inv_largest_possible_distance_3d;
}

// Thanks https://www.ronja-tutorials.com/post/028-voronoi-noise/
template <class Randomizer = DefaultRandomizer>
forceinline f32 voronoi_edge_v3f(v3f coordinate){
	v3f baseCell = floor(coordinate);

	//first pass to find the closest cell
	float minDistToCell = 10;
	v3f toClosestCell;
	v3f closestCell;
	for(f32 x1=-1; x1<=1; x1++){
		for(f32 y1=-1; y1<=1; y1++){
			for(f32 z1=-1; z1<=1; z1++){
				v3f cell = baseCell + v3f{x1, y1, z1};
				v3f cellPosition = cell + Randomizer{}.template random<v3f>(cell);
				v3f toCell = cellPosition - coordinate;
				float distToCell = length(toCell);
				if(distToCell < minDistToCell){
					minDistToCell = distToCell;
					closestCell = cell;
					toClosestCell = toCell;
				}
			}
		}
	}

	//second pass to find the distance to the closest edge
	float minEdgeDistance = 10;
	for(f32 x2=-1; x2<=1; x2++){
		for(f32 y2=-1; y2<=1; y2++){
			for(f32 z2=-1; z2<=1; z2++){
				v3f cell = baseCell + v3f{x2, y2, z2};
				v3f cellPosition = cell + Randomizer{}.template random<v3f>(cell);
				v3f toCell = cellPosition - coordinate;

				v3f diffToClosestCell = absolute(closestCell - cell);
				bool isClosestCell = diffToClosestCell.x + diffToClosestCell.y + diffToClosestCell.z < 0.1;
				if(!isClosestCell){
					v3f toCenter = (toClosestCell + toCell) * 0.5;
					v3f cellDifference = normalize(toCell - toClosestCell);
					float edgeDistance = dot(toCenter, cellDifference);
					minEdgeDistance = min(minEdgeDistance, edgeDistance);
				}
			}
		}
	}

	return minEdgeDistance;
}

template <class Randomizer = DefaultRandomizer>
forceinline f32 voronoi_line_v3f(v3f coordinate) {
	v3f tile = floor(coordinate);
	v3f local = coordinate - tile;

	f32 d = 100.;

	v3f r[3][3][3];

	for (int x = -1; x <= 1; ++x) {
		for (int y = -1; y <= 1; ++y) {
			for (int z = -1; z <= 1; ++z) {
				int ix = x + 1;
				int iy = y + 1;
				int iz = z + 1;
				v3f offset = V3f(x,y,z);
				r[ix][iy][iz] = offset + Randomizer{}.template random<v3f>(tile + offset);
			}
		}
	}

	for (int x = 0; x < 3; ++x) {
		for (int y = 0; y < 3; ++y) {
			for (int z = 0; z < 2; ++z) {
				d = min(d, distance(local, line_segment{r[x][y][z], r[x][y][z+1]}));
				d = min(d, distance(local, line_segment{r[x][z][y], r[x][z+1][y]}));
				d = min(d, distance(local, line_segment{r[z][x][y], r[z+1][x][y]}));
			}
		}
	}

	return d;
}
/*
static const simd_vector::v3sx8 voronoi_line_v3s_offsets_x8[] = {
	simd_typed::S32x8(-1,-1,-1,-1, -1,-1,-1,-1),
	simd_typed::S32x8(-1,-1,-1,+0, +0,+0,+1,+1),
	simd_typed::S32x8(-1,+0,+1,-1, +0,+1,-1,+0),

	simd_typed::S32x8(-1,+0,+0,+0, +0,+0,+0,+0),
	simd_typed::S32x8(+1,-1,-1,-1, +0,+0,+0,+1),
	simd_typed::S32x8(+1,-1,+0,+1, -1,+0,+1,-1),

	simd_typed::S32x8(+0,+0,+1,+1, +1,+1,+1,+1),
	simd_typed::S32x8(+1,+1,-1,-1, -1,+0,+0,+0),
	simd_typed::S32x8(+0,+1,-1,+0, +1,-1,+0,+1),

	simd_typed::S32x8(+1,+1,+1,+1, +1,+1,+1,+1), // NOTE: duplicates to fully fit
	simd_typed::S32x8(+1,+1,+1,+1, +1,+1,+1,+1),
	simd_typed::S32x8(-1,+0,+1,+1, +1,+1,+1,+1),
};
*/

namespace simd_typed {};
namespace simd_vector {};

template <class Randomizer = DefaultRandomizer>
forceinline f32 voronoi_line_v3s(v3s coordinate, s32 step) {
	v3s scaled_tile = floor(coordinate, step);
	v3s tile = scaled_tile / step;
	v3f local = (v3f)(coordinate - scaled_tile) * reciprocal((f32)step);

	f32 d = 100.;

	v3f r[3][3][3];

	using namespace simd_typed;
	using namespace simd_vector;

	if constexpr (false) { //TL_USE_SIMD && ARCH_AVX && requires(Randomizer r) { { r.random_v3fx8(v3sx8{}) } -> std::same_as<v3fx8>; }) {
		//for (auto offset : voronoi_line_v3s_offsets_x8) {
		//	s32x8 ix = offset.x + 1;
		//	s32x8 iy = offset.y + 1;
		//	s32x8 iz = offset.z + 1;
		//	s32x8 in = ix*3*3 + iy*3 + iz;
		//
		//	auto n = (v3fx8)offset + Randomizer{}.template random<v3fx8>(tile + offset);
		//	for(int i = 0; i < 8; ++i)
		//		((v3f *)r)[in[i]] = n.subvector(i);
		//}
	} else {
		static_assert(requires(Randomizer r) { { r.random_v3f(v3s{}) } -> std::same_as<v3f>; }, "random_v3f / random_v3fx8 must be defined");
		for (int x = -1; x <= 1; ++x) {
			for (int y = -1; y <= 1; ++y) {
				for (int z = -1; z <= 1; ++z) {
					int ix = x + 1;
					int iy = y + 1;
					int iz = z + 1;
					v3s offset = V3s(x,y,z);
					r[ix][iy][iz] = (v3f)offset + Randomizer{}.template random<v3f>(tile + offset);
				}
			}
		}
	}

	for (int x = 0; x < 3; ++x) {
		for (int y = 0; y < 3; ++y) {
			for (int z = 0; z < 2; ++z) {
				d = min(d, distance(local, line_segment{r[x][y][z], r[x][y][z+1]}));
				d = min(d, distance(local, line_segment{r[x][z][y], r[x][z+1][y]}));
				d = min(d, distance(local, line_segment{r[z][x][y], r[z+1][x][y]}));
			}
		}
	}

	return d;
}

template <class Result, class Randomizer = DefaultRandomizer>
forceinline Result value_noise(f32 tile_position, f32 local_position) {
	Result left  = Randomizer{}.template random<Result>(tile_position + 0);
	Result right = Randomizer{}.template random<Result>(tile_position + 1);
	return lerp(left, right, local_position);
}
template <class Result, class Randomizer = DefaultRandomizer>
forceinline Result value_noise(v2f tile_position, v2f local_position) {
	Result top_left     = Randomizer{}.template random<Result>(tile_position + v2f{0,0});
	Result bottom_left  = Randomizer{}.template random<Result>(tile_position + v2f{0,1});
	Result bottom_right = Randomizer{}.template random<Result>(tile_position + v2f{1,1});
	Result top_right    = Randomizer{}.template random<Result>(tile_position + v2f{1,0});

	f32 tx = local_position.x;
	f32 ty = local_position.y;

	return lerp(lerp(top_left,    top_right,    tx),
		lerp(bottom_left, bottom_right, tx), ty);
}
template <class Result, class Randomizer = DefaultRandomizer>
forceinline Result value_noise(v3f tile_position, v3f local_position) {
	Result left_bottom_front  = Randomizer{}.template random<Result>(tile_position + v3f{0,0,0});
	Result left_bottom_back   = Randomizer{}.template random<Result>(tile_position + v3f{0,0,1});
	Result left_top_front     = Randomizer{}.template random<Result>(tile_position + v3f{0,1,0});
	Result left_top_back      = Randomizer{}.template random<Result>(tile_position + v3f{0,1,1});
	Result right_bottom_front = Randomizer{}.template random<Result>(tile_position + v3f{1,0,0});
	Result right_bottom_back  = Randomizer{}.template random<Result>(tile_position + v3f{1,0,1});
	Result right_top_front    = Randomizer{}.template random<Result>(tile_position + v3f{1,1,0});
	Result right_top_back     = Randomizer{}.template random<Result>(tile_position + v3f{1,1,1});

	f32 tx = local_position.x;
	f32 ty = local_position.y;
	f32 tz = local_position.z;

	return lerp(lerp(lerp(left_bottom_front, right_bottom_front, tx), 
		             lerp(left_top_front,    right_top_front,    tx), ty),
		        lerp(lerp(left_bottom_back,  right_bottom_back,  tx),
		             lerp(left_top_back,     right_top_back,     tx), ty), tz);
}
template <class Result, class Randomizer = DefaultRandomizer, class Interpolate = decltype([](f32 f){ return f; })>
forceinline Result value_noise(v2s coordinate, s32 step, Interpolate interpolate = {}) {
	v2s floored = floor(coordinate, step);
	v2s tile = floored / step;
	v2f local = (v2f)(coordinate - floored) * reciprocal((f32)step);

	Result top_left     = Randomizer{}.template random<Result>(tile + v2s{0,0});
	Result bottom_left  = Randomizer{}.template random<Result>(tile + v2s{0,1});
	Result bottom_right = Randomizer{}.template random<Result>(tile + v2s{1,1});
	Result top_right    = Randomizer{}.template random<Result>(tile + v2s{1,0});

	f32 tx = interpolate(local.x);
	f32 ty = interpolate(local.y);
	Result top    = lerp(   top_left,    top_right, tx);
	Result bottom = lerp(bottom_left, bottom_right, tx);

	return lerp(top, bottom, ty);
}
template <class Result, class Randomizer = DefaultRandomizer, class Interpolate = decltype([](f32 f){ return f; })>
forceinline Result value_noise(v3s coordinate, s32 step, Interpolate interpolate = {}) {
	static_assert(std::is_same_v<decltype(interpolate(0.0f)), f32>);
	v3s floored = floor(coordinate, step);
	v3s tile = floored / step;
	v3f local = (v3f)(coordinate - floored) * reciprocal((f32)step);

	f32 tx = interpolate(local.x);
	f32 ty = interpolate(local.y);
	f32 tz = interpolate(local.z);
#if 0//TL_USE_SIMD && ARCH_AVX2

	// TODO: use Randomizer

	using namespace simd_typed;
	s32x8 px = S32x8(tile.x) + S32x8(0, 1, 0, 1, 0, 1, 0, 1);
	s32x8 py = S32x8(tile.y) + S32x8(0, 0, 1, 1, 0, 0, 1, 1);
	s32x8 pz = S32x8(tile.z) + S32x8(0, 0, 0, 0, 1, 1, 1, 1);

	auto random_u32x8 = [] (s32x8 s) {
		s ^= S32x8(0x55555555u);
		s *= S32x8(random_primes_u32[0]);
		s ^= S32x8(0x33333333u);
		s *= S32x8(random_primes_u32[1]);
		return s;
	};
	s32x8 s = random_u32x8(px);
	s ^= random_u32x8(py + s);
	s ^= random_u32x8(pz + s);


	f32x8 samples = (f32x8)(s >> 8) * F32x8(1.0f / ((1 << 24) - 1));

	f32x4 a = samples.x4[0];
	f32x4 b = samples.x4[1];

	a = fmadd(b - a, F32x4(tz), a);

	b = {_mm_shuffle_ps(a.m, a.m, _MM_SHUFFLE(0, 0, 3, 2))};
	a = fmadd(b - a, F32x4(ty), a);

	b = {_mm_shuffle_ps(a.m, a.m, _MM_SHUFFLE(0, 0, 0, 1))};
	a = fmadd(b - a, F32x4(tx), a);

	return a.s[0];
#else
	Result left_bottom_back   = Randomizer{}.template random<Result>(tile + v3s{0,0,0});
	Result right_bottom_back  = Randomizer{}.template random<Result>(tile + v3s{1,0,0});
	Result left_top_back      = Randomizer{}.template random<Result>(tile + v3s{0,1,0});
	Result right_top_back     = Randomizer{}.template random<Result>(tile + v3s{1,1,0});
	Result left_bottom_front  = Randomizer{}.template random<Result>(tile + v3s{0,0,1});
	Result right_bottom_front = Randomizer{}.template random<Result>(tile + v3s{1,0,1});
	Result left_top_front     = Randomizer{}.template random<Result>(tile + v3s{0,1,1});
	Result right_top_front    = Randomizer{}.template random<Result>(tile + v3s{1,1,1});

	Result left_bottom  = lerp(left_bottom_back,  left_bottom_front,  tz);
	Result right_bottom = lerp(right_bottom_back, right_bottom_front, tz);
	Result left_top     = lerp(left_top_back,     left_top_front,     tz);
	Result right_top    = lerp(right_top_back,    right_top_front,    tz);

	Result left  = lerp(left_bottom,  left_top,  ty);
	Result right = lerp(right_bottom, right_top, ty);

	return lerp(left, right, tx);
#endif
}

template <class Result, class Randomizer = DefaultRandomizer> forceinline Result value_noise_linear(f32 position) { return value_noise<Result, Randomizer>(floor(position), frac(position)); }
template <class Result, class Randomizer = DefaultRandomizer> forceinline Result value_noise_linear(v2f position) { return value_noise<Result, Randomizer>(floor(position), frac(position)); }
template <class Result, class Randomizer = DefaultRandomizer> forceinline Result value_noise_linear(v3f position) { return value_noise<Result, Randomizer>(floor(position), frac(position)); }
template <class Result, class Randomizer = DefaultRandomizer> forceinline Result value_noise_smooth(f32 position) { return value_noise<Result, Randomizer>(floor(position), smoothstep3(frac(position))); }
template <class Result, class Randomizer = DefaultRandomizer> forceinline Result value_noise_smooth(v2f position) { return value_noise<Result, Randomizer>(floor(position), smoothstep3(frac(position))); }
template <class Result, class Randomizer = DefaultRandomizer> forceinline Result value_noise_smooth(v3f position) { return value_noise<Result, Randomizer>(floor(position), smoothstep3(frac(position))); }

template <class Randomizer = DefaultRandomizer>
forceinline f32 gradient_noise(f32 coordinate) {
	f32 tile = floor(coordinate);
	f32 local = frac(coordinate);

	f32 t0 = local;
	f32 t1 = t0 - 1.f;

	static constexpr f32 directions[] = {-1, -.75, -.5, -.25, .25, .5, .75, 1};
	static_assert(is_power_of_2(count_of(directions)));
	auto get_direction = [&](f32 offset) { return directions[(Randomizer{}.template random<u32>(tile + offset) >> 26) & (count_of(directions) - 1)]; };
	f32 d0 = get_direction(0);
	f32 d1 = get_direction(1);

	f32 v0 = d0 * t0;
	f32 v1 = d1 * t1;

	f32 t = smoothstep3(local);
	return lerp(v0, v1, t) + 0.5f;
}

template <class Randomizer = DefaultRandomizer>
forceinline f32 gradient_noise(v2f coordinate) {
	v2f tile = floor(coordinate);
	v2f local = frac(coordinate);

	v2f t0 = local;
	v2f t1 = t0 - 1.f;

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
	auto get_direction = [&](v2f offset) { return directions[(Randomizer{}.template random<u32>(tile + offset) >> 13) & (count_of(directions) - 1)]; };
	v2f g00 = get_direction(v2f{0, 0});
	v2f g10 = get_direction(v2f{1, 0});
	v2f g01 = get_direction(v2f{0, 1});
	v2f g11 = get_direction(v2f{1, 1});

	f32 v00 = g00.x * t0.x + g00.y * t0.y;
	f32 v10 = g10.x * t1.x + g10.y * t0.y;
	f32 v01 = g01.x * t0.x + g01.y * t1.y;
	f32 v11 = g11.x * t1.x + g11.y * t1.y;

	v2f t = smoothstep3(local);
	return lerp(lerp(v00, v10, t.x), lerp(v01, v11, t.x), t.y) * (sqrt2 / 2.0f) + 0.5f;
}

template <class Randomizer = DefaultRandomizer>
forceinline f32 gradient_noise(v3f coordinate) {
	v3f tile = floor(coordinate);
	v3f local = coordinate - tile;

	v3f t0 = local;
	v3f t1 = t0 - 1;

	static constexpr v3f directions[] = {
		normalize(v3f{ 1, 1, 1}),
		normalize(v3f{-1, 1, 1}),
		normalize(v3f{ 1,-1, 1}),
		normalize(v3f{-1,-1, 1}),
		normalize(v3f{ 1, 1,-1}),
		normalize(v3f{-1, 1,-1}),
		normalize(v3f{ 1,-1,-1}),
		normalize(v3f{-1,-1,-1})
	};
	static_assert(is_power_of_2(count_of(directions)));

	v3f t = smoothstep3(local);

	auto get_direction = [&](v3f offset) { return directions[(Randomizer{}.template random<u32>(tile + offset) >> 13) & (count_of(directions) - 1)]; };
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

template <class Randomizer = DefaultRandomizer>
forceinline f32 gradient_noise(v2s coordinate, s32 step) {
	v2s floored = floor(coordinate, step);
	v2s tile = floored / step;
	v2f local = (v2f)(coordinate - floored) * reciprocal((f32)step);

	v2f t0 = local;
	v2f t1 = t0 - 1.f;

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
	auto get_direction = [&](v2s offset) { return directions[(Randomizer{}.template random<u32>(tile + offset) >> 13) & (count_of(directions) - 1)]; };
	v2f g00 = get_direction(v2s{0, 0});
	v2f g10 = get_direction(v2s{1, 0});
	v2f g01 = get_direction(v2s{0, 1});
	v2f g11 = get_direction(v2s{1, 1});

	f32 v00 = dot(g00, v2f{t0.x, t0.y});
	f32 v10 = dot(g10, v2f{t1.x, t0.y});
	f32 v01 = dot(g01, v2f{t0.x, t1.y});
	f32 v11 = dot(g11, v2f{t1.x, t1.y});

	v2f t = smoothstep3(local);
	return lerp(lerp(v00, v10, t.x), lerp(v01, v11, t.x), t.y) * (sqrt2 / 2.0f) + 0.5f;
}

template <class Randomizer = DefaultRandomizer>
forceinline f32 gradient_noise(v3s coordinate, s32 step) {
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
		normalize(v3f{ 1, 1, 1}),
		normalize(v3f{-1, 1, 1}),
		normalize(v3f{ 1,-1, 1}),
		normalize(v3f{-1,-1, 1}),
		normalize(v3f{ 1, 1,-1}),
		normalize(v3f{-1, 1,-1}),
		normalize(v3f{ 1,-1,-1}),
		normalize(v3f{-1,-1,-1})
	};
	static_assert(is_power_of_2(count_of(directions)));

	v3f t = smoothstep3(local);

	auto get_direction = [&](v3s offset) { return directions[(Randomizer{}.template random<u32>(tile + offset) >> 13) & (count_of(directions) - 1)]; };
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

template <class Generator>
v3f random_unit_v3f(Generator &generator) {
    f32 u1 = next_f32(generator);
    f32 u2 = next_f32(generator);

    // Convert to spherical coordinates
    f32 theta = 2 * pi * u1; // azimuthal angle
    f32 phi = acosf(2 * u2 - 1); // polar angle

    // Convert to Cartesian coordinates
	return {
		sinf(phi) * cosf(theta),
		sinf(phi) * sinf(theta),
		cosf(phi),
	};
}

}

#pragma warning(pop)
