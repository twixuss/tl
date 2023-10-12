#define TL_IMPL
#include <tl/common.h>
#include <tl/file.h>
#include <tl/console.h>
#include <tl/main.h>

using namespace tl;

s32 tl_main(Span<Span<utf8>> args) {
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
		{"+"s, "add"s,   false, M_UINT|M_SINT|M_FLOAT},
		{"-"s, "sub"s,   false, M_UINT|M_SINT|M_FLOAT},
		{"*"s, "mul"s,   false, M_FLOAT},
		{"*"s, "mullo"s, false, M_UINT|M_SINT},
		{"/"s, "div"s,   false, M_UINT|M_SINT|M_FLOAT},
		//{"%"s, "mod"s,   false, M_UINT|M_SINT},
		{"^"s, "xor"s,   true,  M_UINT|M_SINT},
		{"&"s, "and"s,   true,  M_UINT|M_SINT},
		{"|"s, "or"s ,   true,  M_UINT|M_SINT},
		{"<<"s, "sllv"s, false, M_UINT|M_SINT},
		{">>"s, "srlv"s, false, M_UINT},
		{">>"s, "srav"s, false, M_SINT},
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

		for (auto &scalar : scalar_types) {
			auto n_scalars_in_register = reg.size / scalar.size;
			auto type = format("{}x{}", scalar.name, n_scalars_in_register);
			append_format(builder, "union {};\n", type);
		}
	}

	// 
	// Definitions
	//
	for (auto reg : register_types) {
		auto reg_bits = reg.size * 8;

		for (auto &scalar : scalar_types) {
			bool might_be_not_supported = reg_bits > 128;
			auto support_condition = [&] {
				switch (reg_bits) {
					case 256: return (scalar.mask & M_FLOAT) ? "ARCH_AVX"s : "ARCH_AVX2"s;
					case 512: return "ARCH_AVX512"s;
				}
				return ""s;
			}();

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
			auto constructor = copy(type);
			constructor[0] = to_upper(constructor[0]);

			auto mm_prefix = [&]() -> Span<char> {
				if (reg_bits == 128)
					return "_mm_"s;
				return format("_mm{}_", reg_bits);
			}();

			auto set_suffix = scalar.suffix;
			if (scalar.mask == M_UINT) {
				set_suffix = format("epi{}"s, scalar.size * 8);
			}

			if (set_suffix == "epi64"s && reg_bits != 512)
				set_suffix = "epi64x"s;

			//
			// Constructor Declarations
			//
			{
				append_format(builder, "forceinline {} {}({});\n", type, constructor, scalar.name);

				append_format(builder, "forceinline {} {}(", type, constructor);
				for (int i = 0; i < n_scalars_in_register; ++i) {
					if (i != 0)
						append(builder, ',');

					append_format(builder, "{}", scalar.name);
				}
				append(builder, ");\n\n");
			}
			//
			// Struct
			//
			{
				append_format(builder, "union {} {{\n", type);
				if (might_be_not_supported)
					append_format(builder, "#if {}\n", support_condition);
				append_format(builder, "	{} m;\n", builtin_type);
				if (might_be_not_supported)
					append(builder, "#endif\n");

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
					if (might_be_not_supported) {
						append_format(builder, "#if {}\n", support_condition);
						append_format(builder, "	forceinline friend {} operator~({} a) {{ return {{{}xor_si{}(a.m, {}set1_epi32(~0))}}; }}\n", type, type, mm_prefix, reg_bits, mm_prefix);
						append(builder, "#else\n");
						append_format(builder, "	forceinline friend {} operator~({} a) {{", type, type);
						for (u32 i = 0; i < 2; ++i) {
							append_format(builder, "a.x{}[{}] = ~a.x{}[{}]; ", n_scalars_in_register/2, i, n_scalars_in_register/2, i);
						}
						append(builder, "return a; }\n");
						append(builder, "#endif\n");
					} else {
						append_format(builder, "	forceinline friend {} operator~({} a) {{ return {{{}xor_si{}(a.m, {}set1_epi32(~0))}}; }}\n", type, type, mm_prefix, reg_bits, mm_prefix);
					}
				}

				// 
				// Binary operations
				//
				for (auto op : binary_operations) {
					if (op.mask & scalar.mask) {
						if (scalar.size == 1 && (op.op == "*"s || op.op == "<<"s || op.op == ">>"s)) {
							// there's no instructions for these, so do it one by one.
							append_format(builder, "	forceinline friend {} operator{}({} a, {} b) {{", type, op.op, type, type);
							for (u32 i = 0; i < n_scalars_in_register; ++i) {
								append_format(builder, "a.s[{}] {}= b.s[{}]; ", i, op.op, i);
							}
							append(builder, "return a; }\n");

							append_format(builder, "	forceinline friend {} operator{}({} a, {} b) {{", type, op.op, type, scalar.name);
							for (u32 i = 0; i < n_scalars_in_register; ++i) {
								append_format(builder, "a.s[{}] {}= b; ", i, op.op);
							}
							append(builder, "return a; }\n");

							append_format(builder, "	forceinline friend {} operator{}({} a, {} b) {{auto r={}(a);", type, op.op, scalar.name, type, constructor);
							for (u32 i = 0; i < n_scalars_in_register; ++i) {
								append_format(builder, "r.s[{}] {}= b.s[{}]; ", i, op.op, i);
							}
							append(builder, "return r; }\n");

							append_format(builder, "	forceinline friend {} &operator{}=({} &a, {} b) {{ return a=a{}b; }}\n", type, op.op, type, type, op.op);
							append_format(builder, "	forceinline friend {} &operator{}=({} &a, {} b) {{ return a=a{}b; }}\n", type, op.op, type, scalar.name, op.op);

							continue;
						}
						
						auto suffix = scalar.suffix;
						if (op.is_bitwise) {
							suffix = format("si{}"s, reg_bits);
						} else if (scalar.mask == M_UINT && (op.op == "+"s || op.op == "-"s || op.op == "*"s || op.op == "<<"s || op.op == ">>"s)) {
							// These are the same for signed and unsigned
							suffix = format("epi{}"s, scalar.size * 8);
						}

						//
						// Vector X Vector
						//

						{
							// Push these values, pop at the end of scope.
							scoped_replace(might_be_not_supported, might_be_not_supported);
							scoped_replace(support_condition, support_condition);

							if (op.op == "<<"s || op.op == ">>"s) {
								if (scalar.size == 2) {
									might_be_not_supported = true;
									support_condition = "ARCH_AVX512"s;
								} else if (scalar.size == 4 || scalar.size == 8) {
									might_be_not_supported = true;
									if (reg_bits < 512) {
										support_condition = "ARCH_AVX2"s;
									} else {
										support_condition = "ARCH_AVX512"s;
									}
								}
							}

							if (op.op == ">>"s && scalar.size == 8 && scalar.mask == M_SINT) {
								might_be_not_supported = true;
								support_condition = "ARCH_AVX512"s;
							}

							auto append_intrinsic = [&] {
								append_format(builder, "	forceinline friend {} operator{}({} a, {} b) {{ return {{{}{}_{}(a.m, b.m)}}; }}\n", type, op.op, type, type, mm_prefix, op.name, suffix);
							};

							if (might_be_not_supported) {
								append_format(builder, "#if {}\n", support_condition);
								append_intrinsic();
								append(builder, "#else\n");
								append_format(builder, "	forceinline friend {} operator{}({} a, {} b) {{", type, op.op, type, type);
								if (reg_bits == 128) {
									for (u32 i = 0; i < n_scalars_in_register; ++i) {
										append_format(builder, "a.s[{}]{}=b.s[{}];", i, op.op, i);
									}
								} else {
									for (u32 i = 0; i < 2; ++i) {
										append_format(builder, "a.x{}[{}] {}= b.x{}[{}]; ", n_scalars_in_register/2, i, op.op, n_scalars_in_register/2, i);
									}
								}
								append(builder, "return a; }\n");
								append(builder, "#endif\n");
							} else {
								append_intrinsic();
							}
						}

						//
						// Vector X Scalar
						//
						{
							// Push these values, pop at the end of scope.
							scoped_replace(might_be_not_supported, might_be_not_supported);
							scoped_replace(support_condition, support_condition);

							if (op.op == ">>"s && scalar.size == 8 && scalar.mask == M_SINT) {
								might_be_not_supported = true;
								support_condition = "ARCH_AVX512"s;
							}

							if ((op.op == "<<"s || op.op == ">>"s)) {
								auto op_name = to_list(op.name);
								op_name.back() = 'i';

								auto append_intrinsic = [&] {
									append_format(builder, "	forceinline friend {} operator{}({} a, {} b) {{ return {{{}{}_{}(a.m, b)}}; }}\n", type, op.op, type, scalar.name, mm_prefix, op_name, suffix);
								};
								if (might_be_not_supported) {
									append_format(builder, "#if {}\n", support_condition);
									append_intrinsic();
									append(builder, "#else\n");
									append_format(builder, "	forceinline friend {} operator{}({} a, {} b) {{", type, op.op, type, scalar.name);
									if (reg_bits == 128) {
										for (u32 i = 0; i < n_scalars_in_register; ++i) {
											append_format(builder, "a.s[{}]{}=b;", i, op.op);
										}
									} else {
										for (u32 i = 0; i < 2; ++i) {
											append_format(builder, "a.x{}[{}] {}= b; ", n_scalars_in_register/2, i, op.op);
										}
									}
									append(builder, "return a; }\n");
									append(builder, "#endif\n");
								} else {
									append_intrinsic();
								}
							} else {
								append_format(builder, "	forceinline friend {} operator{}({} a, {} b) {{ return a{}{}(b); }}\n", type, op.op, type, scalar.name, op.op, constructor);
							}
							append_format(builder, "	forceinline friend {} operator{}({} a, {} b) {{ return {}(a){}b; }}\n", type, op.op, scalar.name, type, constructor, op.op);
							append_format(builder, "	forceinline friend {} &operator{}=({} &a, {} b) {{ return a=a{}b; }}\n", type, op.op, type, type, op.op);
							append_format(builder, "	forceinline friend {} &operator{}=({} &a, {} b) {{ return a=a{}b; }}\n", type, op.op, type, scalar.name, op.op);
						}
					}
				}

				// Shifts
				//if (scalar.mask & (M_UINT|M_SINT)) {
				//	if (scalar.size == 1) {
				//		with(ConsoleColor::red, println("NOT IMPLEMENTED"));
				//	} else {
				//		auto append_with_intrinsic = [&] {
				//			auto suffix = format("epi{}"s, scalar.size * 8);
				//			append_format(builder, "	forceinline friend {} operator<<({} a, {} b) {{ return {{{}slli_{}(a.m, b)}}; }}\n", type, type, scalar.name, mm_prefix, suffix);
				//			append_format(builder, "	forceinline friend {} operator>>({} a, {} b) {{ return {{{}sr{}i_{}(a.m, b)}}; }}\n", type, type, scalar.name, mm_prefix, scalar.mask==M_UINT?"l":"a", suffix);
				//		};
				//		if (might_be_not_supported) {
				//			append_format(builder, "#if {}\n", support_condition);
				//			append_with_intrinsic();
				//			append(builder, "#else\n");
				//			append(builder, "#endif\n");
				//		} else {
				//			append_with_intrinsic();
				//		}
				//	}
				//}

				// Cast Declarations
				for (auto &other_scalar : scalar_types) {
					if (&scalar == &other_scalar)
						continue;

					for (auto other_reg : register_types) {
						auto n_other_scalars_in_register = other_reg .size / other_scalar.size;
						if (n_scalars_in_register != n_other_scalars_in_register)
							continue;

						auto other_type = format("{}x{}", other_scalar.name, n_other_scalars_in_register);

						append_format(builder, "	forceinline operator {}() const;\n", other_type);
					}
				}

				append_format(builder, "}};\n\n");
			}

			//
			// Constructors
			//
			{
				auto append_constructor_with_intrinsic = [&] {
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
					append_format(builder, ")}}; }}\n");
				};
				if (might_be_not_supported) {
					auto sub_constructor = format("{}x{}", scalar.name, n_scalars_in_register/2);
					sub_constructor[0] = to_upper(sub_constructor[0]);

					append_format(builder, "#if {}\n", support_condition);
					append_constructor_with_intrinsic();
					append(builder, "#else\n");
					append_format(builder, "forceinline {} {}({} a){{{} r;", type, constructor, scalar.name, type);
					for (int i = 0; i < n_scalars_in_register; ++i) {
						append_format(builder, "r[{}]=a;", i);
					}
					append(builder, "return r;}\n");
					append_format(builder, "forceinline {} {}(", type, constructor);
					for (int i = 0; i < n_scalars_in_register; ++i) {
						if (i != 0)
							append(builder, ',');

						append_format(builder, "{} _{}", scalar.name, i);
					}
					append_format(builder, "){{{} r;", type);
					for (int i = 0; i < n_scalars_in_register; ++i) {
						append_format(builder, "r[{}]=_{};", i, i);
					}
					append(builder, "return r;}\n");
					append(builder, "#endif\n");
				} else {
					append_constructor_with_intrinsic();
				}

			}

			// 
			// Math functions
			//
			{
				// 
				// fmadd
				//
				{
					if (scalar.mask == M_FLOAT) {
						// Push these values, pop at the end of scope.
						scoped_replace(might_be_not_supported, might_be_not_supported);
						scoped_replace(support_condition, support_condition);

						switch (reg_bits) {
							case 128:
							case 256:
								might_be_not_supported = true;
								support_condition = "ARCH_AVX"s;
								break;
							case 512:
								might_be_not_supported = true;
								support_condition = "ARCH_AVX512"s;
								break;
						}

						auto append_intrinsic = [&] {
							append_format(builder, "forceinline {} fmadd({} a, {} b, {} c) {{ return {{{}fmadd_{}(a.m, b.m, c.m)}}; }}\n", type, type, type, type, mm_prefix, scalar.suffix);
						};
						if (might_be_not_supported) {
							append_format(builder, "#if {}\n", support_condition);
							append_intrinsic();
							append(builder, "#else\n");
							append_format(builder, "forceinline {} fmadd({} a, {} b, {} c) {{", type, type, type, type);
							for (int i = 0; i < n_scalars_in_register; ++i) {
								append_format(builder, "a.s[{}]=a.s[{}]*b.s[{}]+c.s[{}];", i, i, i, i);
							}
							append(builder, "return a;}\n#endif\n");
						} else {
							append_intrinsic();
						}
		
						append(builder, '\n');
					}
				}
			}
		}

		// Cast definitions
		for (auto &scalar : scalar_types) {
			auto n_scalars_in_register = reg.size / scalar.size;
			auto type = format("{}x{}", scalar.name, n_scalars_in_register);

			{
				for (auto &other_scalar : scalar_types) {
					if (&scalar == &other_scalar)
						continue;

					for (auto other_reg : register_types) {
						auto n_other_scalars_in_register = other_reg.size / other_scalar.size;
						if (n_scalars_in_register != n_other_scalars_in_register)
							continue;

						auto mm_prefix = [&]() -> Span<char> {
							auto bits = max(reg_bits, other_reg.size*8);
							if (bits == 128)
								return "_mm_"s;
							return format("_mm{}_", bits);
						}();

						auto other_type = format("{}x{}", other_scalar.name, n_other_scalars_in_register);

						if ((scalar.mask & (M_UINT|M_SINT)) && (other_scalar.mask & (M_UINT|M_SINT))) {
							// INT <-> UINT is noop
							append_format(builder, "forceinline {}::operator {}() const {{ return *({}*)this; }}\n", type, other_type, other_type);
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
							if (scalar.mask == M_FLOAT && (other_scalar.mask&(M_UINT|M_SINT))) {
								append_format(builder, "forceinline {}::operator {}() const {{ return {{{}cvtt{}_{}(m)}}; }}\n", type, other_type, mm_prefix, scalar.suffix, other_scalar.suffix);

							} else {
								append_format(builder, "forceinline {}::operator {}() const {{ return {{{}cvt{}_{}(m)}}; }}\n", type, other_type, mm_prefix, scalar.suffix, other_scalar.suffix);
							}
						}
					}
				}
			}
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

			for (u32 dim = 2; dim <= 4; ++dim) {
				for (auto &scalar : scalar_types) {
					auto n_scalars_in_register = reg.size / scalar.size;
					append_format(builder, "union v{}{}x{};\n", dim, scalar.name, n_scalars_in_register);
				}
			}

		}
	}

	// Definitions
	{
		for (auto reg : register_types) {
			auto reg_bits = reg.size * 8;

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
