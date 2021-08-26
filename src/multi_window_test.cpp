#define TL_MAIN
#define TL_IMPL
#include <tl/common.h>
#include <tl/window.h>

using namespace tl;

s32 tl_main(Span<Span<utf8>> arguments) {

	auto main_window = create_window({
		.title = u8"Main window"s,
	});

	auto child_window = create_window({
		.title = u8"Child window"s,
		.style_flags = WindowStyle_no_frame,
		.parent = main_window,
		.on_draw = [](Window &window) {
			draw_rect(window, 8, 8, 16, 16);
		},
		.hit_test = [](Window &window) -> WindowHitResult {
			auto mx = window.mouse_position.x;
			auto my = window.mouse_position.y;
			auto sx = window.client_size.x;
			auto sy = window.client_size.y;
			auto b = 8;
			if (my < b) {
				if (mx < b) {
					return WindowHit_top_left;
				} else if (mx >= sx - b) {
					return WindowHit_top_right;
				}
				return WindowHit_top;
			} else if (my >= sy - b) {
				if (mx < b) {
					return WindowHit_bottom_left;
				} else if (mx >= sx - b) {
					return WindowHit_bottom_right;
				}
				return WindowHit_bottom;
			}
			return WindowHit_title;
		},
	});

	while (update(main_window)) {}

	return 0;
}
