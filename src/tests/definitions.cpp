#define TL_ENABLE_TESTS
#ifdef _WIN32
#define NOMINMAX
#include <WinSock2.h>
#endif

#define TL_IMPL
#include <tl/system.h>

#if OS_WINDOWS
#include <tl/win32.h>
#include <tl/win32_error.h>
#include <tl/win32_message.h>
#include <tl/d3d11.h>
#endif

#include <tl/array.h>
#include <tl/array2.h>
#include <tl/array3.h>
#include <tl/atlas.h>
#include <tl/big_int.h>
#include <tl/big_rational.h>
#include <tl/bin2cpp.h>
#include <tl/bits.h>
#include <tl/block_list.h>
#include <tl/block_list_no_stack.h>
#include <tl/buffer.h>
#include <tl/c_version.h>
#include <tl/clipboard.h>
#include <tl/color.h>
#include <tl/common.h>
#include <tl/compiler.h>
#include <tl/complex.h>
#include <tl/console.h>
#include <tl/constexpr_string_literal.h>
#include <tl/coroutine.h>
#include <tl/cpu.h>
#include <tl/d3d11.h>
#include <tl/debug.h>
#include <tl/debug_allocator.h>
#include <tl/default_logger.h>
#include <tl/dynamic_lib.h>
#include <tl/fft.h>
#include <tl/fiber.h>
#include <tl/file.h>
#include <tl/file_printer.h>
#include <tl/file_stream.h>
#include <tl/fly_string.h>
#include <tl/font.h>
#include <tl/function.h>
#include <tl/gltf.h>
#include <tl/hash.h>
#include <tl/contiguous_hash_map.h>
#include <tl/contiguous_hash_set.h>
#include <tl/bucket_hash_map.h>
#include <tl/bucket_hash_set.h>
#include <tl/signed_int.h>
#include <tl/static_bucket_hash_map.h>
#include <tl/hash_map.h>
#include <tl/hash_set.h>
#include <tl/image_filters.h>
#include <tl/includer.h>
#include <tl/input.h>
#include <tl/json.h>
#include <tl/linked_list.h>
#include <tl/list.h>
#include <tl/list_of_lists.h>
#include <tl/logger.h>
#include <tl/macros.h>
#include <tl/masked_block_list.h>
#include <tl/math.h>
#include <tl/math_random.h>
#include <tl/memory.h>
#include <tl/memory_stream.h>
#include <tl/mesh.h>
#include <tl/msvc.h>
#include <tl/net.h>
#include <tl/opengl.h>
#include <tl/pointer.h>
#include <tl/pool32.h>
#include <tl/process.h>
#include <tl/profiler.h>
#include <tl/protected_fly_string.h>
#include <tl/qoi.h>
#include <tl/quaternion.h>
#include <tl/ram.h>
#include <tl/random.h>
#include <tl/reusable_fiber.h>
#include <tl/ring_buffer.h>
#include <tl/simd.h>
#include <tl/sorted_list.h>
#include <tl/source_location.h>
#include <tl/static_queue.h>
#include <tl/static_ring_buffer.h>
#include <tl/stream.h>
#include <tl/string.h>
#include <tl/text_input_field.h>
#include <tl/thread.h>
#include <tl/time.h>
#include <tl/tracking_allocator.h>
#include <tl/turn.h>
#include <tl/u256.h>
#include <tl/variant.h>
#include <tl/vector.h>
#include <tl/window.h>


void run_tl_tests() {
	using namespace tl;
	for (int i = 0; i < tests_to_run_count; i++) {
		println(tests_to_run[i].name);
		tests_to_run[i].func();
	}

}
static tl::gl::Functions opengl_functions;
tl::gl::Functions *tl_opengl_functions() {
	return &opengl_functions;
}