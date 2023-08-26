#define TL_IMPL
#include <tl/common.h>
#include <tl/file.h>
#include <tl/console.h>
#include <tl/main.h>

using namespace tl;

s32 tl_main(Span<Span<utf8>> args) {
	current_allocator = temporary_allocator;

	u32 const M_UINT  = 0x1;
	u32 const M_SINT  = 0x2;
	u32 const M_FLOAT = 0x4;

	struct Scalar {
		Span<char> name;
		Span<char> suffix;
		u32 size;
		u32 mask;
	};

	Scalar scalar_types[] {
		{"u8"s,  "epu8"s,  1, M_UINT },
		{"u16"s, "epu16"s, 2, M_UINT },
		{"u32"s, "epu32"s, 4, M_UINT },
		{"u64"s, "epu64"s, 8, M_UINT },
		{"s8"s,  "epi8"s,  1, M_UINT },
		{"s16"s, "epi16"s, 2, M_SINT },
		{"s32"s, "epi32"s, 4, M_SINT },
		{"s64"s, "epi64"s, 8, M_SINT },
		{"f32"s, "ps"s,    4, M_FLOAT},
		{"f64"s, "pd"s,    8, M_FLOAT},
	};

	struct Register {
		u32 size;
	};

	Register register_types[] {
		{16},
		{32},
		{64},
	};

	struct BinaryOperation {
		Span<char> op;
		Span<char> name;
		bool is_bitwise;
		u32 mask;
	};

	BinaryOperation binary_operations[] {
		{"+"s, "add"s, false, M_UINT|M_SINT|M_FLOAT},
		{"-"s, "sub"s, false, M_UINT|M_SINT|M_FLOAT},
		{"*"s, "mul"s, false, M_UINT|M_SINT|M_FLOAT},
		{"/"s, "div"s, false, M_UINT|M_SINT|M_FLOAT},
		//{"%"s, "mod"s, false, M_UINT|M_SINT        },
		{"^"s, "xor"s, true,  M_UINT|M_SINT        },
		{"&"s, "and"s, true,  M_UINT|M_SINT        },
		{"|"s, "or"s , true,  M_UINT|M_SINT        },
	};

	StringBuilder builder;

	append(builder, R"(#pragma once
#include <tl/common.h>

namespace tl {
namespace simd_typed {

)");

	// 
	// Forward Declarations
	//
	for (auto reg : register_types) {
		auto reg_bits = reg.size * 8;
		switch (reg_bits) {
			case 256: append(builder, "\n#if ARCH_AVX\n"); break;
			case 512: append(builder, "\n#if ARCH_AVX512\n"); break;
		}


		for (auto &scalar : scalar_types) {
			auto n_scalars_in_register = reg.size / scalar.size;
			auto type = format("{}x{}", scalar.name, n_scalars_in_register);
			append_format(builder, "union {};\n", type);
		}

		switch (reg_bits) {
			case 256:
			case 512: append(builder, "\n#endif\n"); break;
		}
	}

	// 
	// Definitions
	//
	for (auto reg : register_types) {
		auto reg_bits = reg.size * 8;
		switch (reg_bits) {
			case 256: append(builder, "\n#if ARCH_AVX\n"); break;
			case 512: append(builder, "\n#if ARCH_AVX512\n"); break;
		}


		for (auto &scalar : scalar_types) {
			auto n_scalars_in_register = reg.size / scalar.size;
			auto builtin_type_suffix = [&](){
				switch (scalar.name[0]) {
					case 'u': 
					case 's': return "i"s; 
				}
				if (scalar.name == "f64"s)
					return "d"s;
				return ""s; 
			}();
			auto builtin_type = format("__m{}{}", reg_bits, builtin_type_suffix);
			auto type = format("{}x{}", scalar.name, n_scalars_in_register);

			auto mm_prefix = [&]() -> Span<char> {
				if (reg_bits == 128)
					return "_mm_"s;
				return format("_mm{}_", reg_bits);
			}();

			auto set_suffix = scalar.suffix;
			if (scalar.mask == M_UINT) {
				set_suffix = format("epi{}"s, scalar.size * 8);
			}

			if (set_suffix == "epi64"s)
				set_suffix = "epi64x"s;

			//
			// Struct
			//
			{
				append_format(builder, "union {} {{\n", type);
				append_format(builder, "	{} m;\n", builtin_type);

				for (int sub = n_scalars_in_register / 2; sub != 1; sub /= 2) {
					auto sub_bits = sub * scalar.size * 8;
					if (sub_bits >= 128)
						append_format(builder, "	{}x{} x{}[{}];\n", scalar.name, sub, sub, n_scalars_in_register/sub);
				}
				
				append_format(builder, "	{} s[{}];\n", scalar.name, n_scalars_in_register);

				append_format(builder, "	forceinline {} operator[](umm i) const {{ return s[i]; }}\n", scalar.name);
				append_format(builder, "	forceinline {} &operator[](umm i) {{ return s[i]; }}\n", scalar.name);
				append_format(builder, "	forceinline friend {} operator+({} a) {{ return a; }}\n", type, type);
				append_format(builder, "	forceinline friend {} operator-({} a) {{ return {}{{}} - a; }}\n", type, type, type);
				if (scalar.mask & (M_UINT | M_SINT)) {
					append_format(builder, "	forceinline friend {} operator~({} a) {{ return {{{}xor_si{}(a.m, {}set1_epi32(~0))}}; }}\n", type, type, mm_prefix, reg_bits, mm_prefix);
				}
				for (auto op : binary_operations) {
					if (op.mask & scalar.mask) {

						if (scalar.size == 1 && op.op == "*"s) {
							// there's no instructions for 8-bit multiplies, do it one by one.
							append_format(builder, "	forceinline friend {} operator*({} a, {} b) {{", type, type, type);
							for (u32 i = 0; i < n_scalars_in_register; ++i) {
								append_format(builder, "(({}*)&a)[{}] *= (({}*)&b)[{}]; ", scalar.name, i, scalar.name, i);
							}
							append(builder, "return a; }\n");

							append_format(builder, "	forceinline friend {} operator*({} a, {} b) {{", type, type, scalar.name);
							for (u32 i = 0; i < n_scalars_in_register; ++i) {
								append_format(builder, "(({}*)&a)[{}] *= b; ", scalar.name, i);
							}
							append(builder, "return a; }\n");

							append_format(builder, "	forceinline friend {} operator*({} a, {} b) {{", type, scalar.name, type);
							for (u32 i = 0; i < n_scalars_in_register; ++i) {
								append_format(builder, "(({}*)&b)[{}] *= a; ", scalar.name, i);
							}
							append(builder, "return b; }\n");

							append_format(builder, "	forceinline friend {} &operator*=({} &a, {} b) {{ return a=a*b; }}\n", type, type, type);
							append_format(builder, "	forceinline friend {} &operator*=({} &a, {} b) {{ return a=a*b; }}\n", type, type, scalar.name);

							continue;
						}

						auto suffix = scalar.suffix;
						if (op.is_bitwise) {
							suffix = format("si{}"s, reg_bits);
						} else if (scalar.mask == M_UINT && (op.op == "+"s || op.op == "-"s || op.op == "*"s)) {
							// Binary + - * are the same for signed and unsigned
							suffix = format("epi{}"s, scalar.size * 8);
						}

						auto op_name = op.name;
						if ((scalar.mask & (M_UINT|M_SINT)) && op.op == "*"s)
							op_name = "mullo"s;

						append_format(builder, "	forceinline friend {} operator{}({} a, {} b) {{ return {{{}{}_{}(a.m, b.m)}}; }}\n", type, op.op, type, type, mm_prefix, op_name, suffix);
						append_format(builder, "	forceinline friend {} operator{}({} a, {} b) {{ return {{{}{}_{}(a.m, {}set1_{}(b))}}; }}\n", type, op.op, type, scalar.name, mm_prefix, op_name, suffix, mm_prefix, set_suffix);
						append_format(builder, "	forceinline friend {} operator{}({} a, {} b) {{ return {{{}{}_{}({}set1_{}(a), b.m)}}; }}\n", type, op.op, scalar.name, type, mm_prefix, op_name, suffix, mm_prefix, set_suffix);
						append_format(builder, "	forceinline friend {} &operator{}=({} &a, {} b) {{ return a=a{}b; }}\n", type, op.op, type, type, op.op);
						append_format(builder, "	forceinline friend {} &operator{}=({} &a, {} b) {{ return a=a{}b; }}\n", type, op.op, type, scalar.name, op.op);
					}
				}

				// Shifts
				if (scalar.mask & (M_UINT|M_SINT)) {
					if (scalar.size != 1) {  // there's no instructions for 8-bit shifts...
						auto suffix = format("epi{}"s, scalar.size * 8);
						append_format(builder, "	forceinline friend {} operator<<({} a, {} b) {{ return {{{}slli_{}(a.m, b)}}; }}\n", type, type, scalar.name, mm_prefix, suffix);
						append_format(builder, "	forceinline friend {} operator>>({} a, {} b) {{ return {{{}sr{}i_{}(a.m, b)}}; }}\n", type, type, scalar.name, mm_prefix, scalar.mask==M_UINT?"l":"a", suffix);
					}
				}

				// Cast Declarations
				for (auto &other_scalar : scalar_types) {
					if (&scalar == &other_scalar)
						continue;
					
					auto n_other_scalars_in_register = reg.size / other_scalar.size;
					if (n_scalars_in_register != n_other_scalars_in_register)
						continue;

					auto other_type = format("{}x{}", other_scalar.name, n_other_scalars_in_register);

					append_format(builder, "	forceinline operator {}() const;\n", other_type);
				}

				append_format(builder, "}};\n\n");
			}

			//
			// Constructors
			//
			{
				auto constructor = format("{}x{}", scalar.name, n_scalars_in_register);
				constructor[0] = to_upper(constructor[0]);

				append_format(builder, "forceinline {} {}({} a) {{ return {{{}set1_{}(a)}}; }}\n", type, constructor, scalar.name, mm_prefix, set_suffix);

				append_format(builder, "forceinline {} {}(", type, constructor);
				for (int i = 0; i < n_scalars_in_register; ++i) {
					if (i != 0)
						append(builder, ',');

					append_format(builder, "{} _{}", scalar.name, i);
				}
				append_format(builder, ") {{ return {{{}setr_{}(", mm_prefix, set_suffix);
				for (int i = 0; i < n_scalars_in_register; ++i) {
					if (i != 0)
						append(builder, ',');

					append_format(builder, "_{}", i);
				}
				append_format(builder, ")}}; }}\n\n");
			}

			// 
			// Math functions
			//
			{
				// 
				// fmadd
				//
				if (scalar.mask == M_FLOAT) {
					append_format(builder, "forceinline {} fmadd({} a, {} b, {} c) {{ return {{{}fmadd_{}(a.m, b.m, c.m)}}; }}\n", type, type, type, type, mm_prefix, scalar.suffix);
				}
		
				append(builder, '\n');
			}
		}

		// Cast definitions
		for (auto &scalar : scalar_types) {
			auto n_scalars_in_register = reg.size / scalar.size;
			auto type = format("{}x{}", scalar.name, n_scalars_in_register);

			auto mm_prefix = [&]() -> Span<char> {
				if (reg_bits == 128)
					return "_mm_"s;
				return format("_mm{}_", reg_bits);
			}();

			{
				for (auto &other_scalar : scalar_types) {
					if (&scalar == &other_scalar)
						continue;

					auto n_other_scalars_in_register = reg.size / other_scalar.size;
					if (n_scalars_in_register != n_other_scalars_in_register)
						continue;

					auto other_type = format("{}x{}", other_scalar.name, n_other_scalars_in_register);

					if ((scalar.mask & (M_UINT|M_SINT)) && (other_scalar.mask & (M_UINT|M_SINT))) {
						// INT <-> UINT is noop
						append_format(builder, "forceinline {}::operator {}() const {{ return {{m}}; }}\n", type, other_type);
					} else if (scalar.mask == M_UINT && other_scalar.mask == M_FLOAT) {
						// UINT -> FLOAT available only in avx512
						append_format(builder, "forceinline {}::operator {}() const {{\n", type, other_type);
						append       (builder, "#if ARCH_AVX512\n");
						append_format(builder, "	return {{{}cvt{}_{}(m)}}; }}\n", mm_prefix, scalar.suffix, other_scalar.suffix);
						append       (builder, "#else\n");
						append_format(builder, "	{} r;\n", other_type);
						for (u32 i = 0; i < n_scalars_in_register; ++i) {
							append_format(builder, "	r[{}] = ({})s[{}];\n", i, other_scalar.name, i);
						}
						append       (builder, "	return r;\n");
						append       (builder, "#endif\n}\n");
					} else {
						// Convert wide
						append_format(builder, "forceinline {}::operator {}() const {{ return {{{}cvt{}_{}(m)}}; }}\n", type, other_type, mm_prefix, scalar.suffix, other_scalar.suffix);
					}
				}
			}
		}

		switch (reg_bits) {
			case 256:
			case 512: append(builder, "\n#endif\n"); break;
		}
	}

	append(builder, R"(
}
}
)");

	write_entire_file(tl_file_string("../include/tl/generated/simd.h"ts), as_bytes(to_string(builder)));

	/////////////////////
	//                 //
	//     VECTORS     //
	//                 //
	/////////////////////

	builder.clear();
	append(builder, R"(#pragma once
#include "simd.h"
#include "../vector.h"

namespace tl {
namespace simd_vector {

)");

	char axes[] {'x', 'y', 'z', 'w'};

	// Forward declarations
	{
		for (auto reg : register_types) {
			auto reg_bits = reg.size * 8;

			switch (reg_bits) {
				case 256: append(builder, "\n#if ARCH_AVX\n"); break;
				case 512: append(builder, "\n#if ARCH_AVX512\n"); break;
			}
			for (u32 dim = 2; dim <= 4; ++dim) {
				for (auto &scalar : scalar_types) {
					auto n_scalars_in_register = reg.size / scalar.size;
					append_format(builder, "union v{}{}x{};\n", dim, scalar.name, n_scalars_in_register);
				}
			}

			switch (reg_bits) {
				case 256:
				case 512: append(builder, "\n#endif\n"); break;
			}
		}
	}

	// Definitions
	{
		for (auto reg : register_types) {
			auto reg_bits = reg.size * 8;
			switch (reg_bits) {
				case 256: append(builder, "\n#if ARCH_AVX\n"); break;
				case 512: append(builder, "\n#if ARCH_AVX512\n"); break;
			}

			for (u32 dim = 2; dim <= 4; ++dim) {
				for (auto &scalar : scalar_types) {
					auto n_scalars_in_register = reg.size / scalar.size;
					auto vec_name = format("v{}{}", dim, scalar.name);
					auto vecx_name = format("{}x{}", vec_name, n_scalars_in_register);
					auto sclx_name = format("simd_typed::{}x{}", scalar.name, n_scalars_in_register);

					append_format(builder, "union {} {{\n", vecx_name);
					append_format(builder, "	struct {{ {} ", sclx_name);
					for (u32 i = 0; i < dim; ++i) {
						if (i != 0)
							append(builder, ", ");
						append(builder, axes[i]);
					}
					append(builder, "; };\n");
					append_format(builder, "	{} d[{}];\n", sclx_name, dim);

					append_format(builder, "	forceinline {} subvector(umm i) const {{ return {{", vec_name);
					for (u32 i = 0; i < dim; ++i) {
						append_format(builder, "{}[i], ", axes[i]);
					}
					append(builder, "}; }\n");

					for (auto op : binary_operations) {
						if (op.mask & scalar.mask) {
							auto append_vec_op = [&] (auto ta, auto tb) {
								append_format(builder, "	forceinline friend {} operator{}({} a, {} b) {{ return {{", vecx_name, op.op, ta, tb);
								for (u32 i = 0; i < dim; ++i) {
									if (i != 0)
										append(builder, ", ");
									append_format(builder, "a.{}{}b.{}", axes[i], op.op, axes[i]);
								}
								append(builder, "};}\n");
							};

							append_vec_op(vecx_name, vecx_name);
							append_vec_op(vecx_name, vec_name);
							append_vec_op(vec_name, vecx_name);



							append_format(builder, "	forceinline friend {} operator{}({} a, {} b) {{ return {{", vecx_name, op.op, vecx_name, scalar.name);
							for (u32 i = 0; i < dim; ++i) {
								if (i != 0)
									append(builder, ", ");
								append_format(builder, "a.{}{}b", axes[i], op.op);
							}
							append(builder, "};}\n");

							append_format(builder, "	forceinline friend {} operator{}({} a, {} b) {{ return {{", vecx_name, op.op, scalar.name, vecx_name);
							for (u32 i = 0; i < dim; ++i) {
								if (i != 0)
									append(builder, ", ");
								append_format(builder, "a{}b.{}", op.op, axes[i]);
							}
							append(builder, "};}\n");
						}
					}

					// Shifts
					if (scalar.mask & (M_UINT|M_SINT)) {
						if (scalar.size != 1) {  // there's no instructions for 8-bit shifts...
							auto suffix = format("epi{}"s, scalar.size * 8);

							append_format(builder, "	forceinline friend {} operator<<({} a, {} b) {{ return {{", vecx_name, vecx_name, scalar.name);
							for (u32 i = 0; i < dim; ++i) {
								if (i != 0)
									append(builder, ", ");
								append_format(builder, "a.{}<<b", axes[i]);
							}
							append(builder, "};}\n");

							append_format(builder, "	forceinline friend {} operator>>({} a, {} b) {{ return {{", vecx_name, vecx_name, scalar.name);
							for (u32 i = 0; i < dim; ++i) {
								if (i != 0)
									append(builder, ", ");
								append_format(builder, "a.{}>>b", axes[i]);
							}
							append(builder, "};}\n");
						}
					}

					// Cast Declarations
					for (auto &other_scalar : scalar_types) {
						if (&scalar == &other_scalar)
							continue;

						auto n_other_scalars_in_register = reg.size / other_scalar.size;
						if (n_scalars_in_register != n_other_scalars_in_register)
							continue;

						auto other_vecx_name = format("v{}{}x{}", dim, other_scalar.name, n_scalars_in_register);

						append_format(builder, "	forceinline operator {}() const;\n", other_vecx_name);
					}

					append(builder, "};\n\n");

					//
					// Constructors
					//
					{
						auto constructor = copy(vecx_name);
						constructor[0] = to_upper(constructor[0]);

						auto scalar_constructor = format("{}x{}", scalar.name, n_scalars_in_register);
						scalar_constructor[0] = to_upper(scalar_constructor[0]);

						append_format(builder, "forceinline {} {}({} a) {{ return {{", vecx_name, constructor, vec_name);
						for (u32 i = 0; i < dim; ++i) {
							append_format(builder, "simd_typed::{}(a.{}), ", scalar_constructor, axes[i]);
						}
						append(builder, "}; }\n");
					}

					// 
					// Math functions
					//
					{
						// 
						// dot
						//
						append_format(builder, "forceinline {} dot({} a, {} b) {{ return ", sclx_name, vecx_name, vecx_name);
						for (u32 i = 0; i < dim; ++i) {
							if (i != 0)
								append(builder, '+');

							append_format(builder, "a.{}*b.{}", axes[i], axes[i]);
						}
						append(builder, "; }\n");

						append(builder, '\n');
					}
				}

				// Cast definitions
				for (auto &scalar : scalar_types) {
					auto n_scalars_in_register = reg.size / scalar.size;
					auto vec_name = format("v{}{}", dim, scalar.name);
					auto vecx_name = format("{}x{}", vec_name, n_scalars_in_register);
					auto sclx_name = format("simd_typed::{}x{}", scalar.name, n_scalars_in_register);

					{
						for (auto &other_scalar : scalar_types) {
							if (&scalar == &other_scalar)
								continue;

							auto n_other_scalars_in_register = reg.size / other_scalar.size;
							if (n_scalars_in_register != n_other_scalars_in_register)
								continue;

							auto other_vec_name = format("v{}{}", dim, other_scalar.name);
							auto other_vecx_name = format("{}x{}", other_vec_name, n_scalars_in_register);
							auto other_sclx_name = format("simd_typed::{}x{}", other_scalar.name, n_scalars_in_register);

							append_format(builder, "forceinline {}::operator {}() const {{ return {{", vecx_name, other_vecx_name);
							for (u32 i = 0; i < dim; ++i) {
								append_format(builder, "({}){}, ", other_sclx_name, axes[i]);
							}
							append(builder, "}; }\n");
						}
					}
				}

			}

			switch (reg_bits) {
				case 256:
				case 512: append(builder, "\n#endif\n"); break;
			}
		}
	}

	append(builder, R"(
using v2sx8 = v2s32x8;
using v2ux8 = v2u32x8;
using v2fx8 = v2f32x8;
using v3sx8 = v3s32x8;
using v3ux8 = v3u32x8;
using v3fx8 = v3f32x8;
using v4sx8 = v4s32x8;
using v4ux8 = v4u32x8;
using v4fx8 = v4f32x8;
}
}
)");

	write_entire_file(tl_file_string("../include/tl/generated/simd_vector.h"ts), as_bytes(to_string(builder)));

	print("Done\n");

	return 0;
}
