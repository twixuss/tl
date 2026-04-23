// This file is intended to be included in tl::Array template specializations
// Why?
// I just decided to have one struct for all - arrays, vectors, simd.
// And vectors require xyzw members, which in turn require different specializations for sizes 2, 3 and 4+.
// So to not duplicate code, put in an h file.

using Element = T;
using Scalar = T; // Compatibility with vectors
using v2 = Array<T, 2>;
using v3 = Array<T, 3>;
using v4 = Array<T, 4>;
