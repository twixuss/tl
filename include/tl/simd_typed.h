#include "generated/simd.h"

namespace tl {
namespace simd_typed {

forceinline f32x8 absolute(f32x8 v) { return *(u32x8*)&v &= 0x7FFFFFFF, v; }

}
}
