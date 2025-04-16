#pragma once
#include "common.h"
#include "math.h"

namespace tl {

template <class Element, class Area, class Allocator = Allocator>
struct Atlas {
	Element *data = 0;
	v2u size = {};
	List<Area, Allocator> areas;
	
	void *user_data = 0;
	void (*on_modify)(Atlas *atlas, void *user_data, aabb<v2u> modified_area) = autocast noop;
	void (*on_resize)(Atlas *atlas, void *user_data, v2u new_size) = autocast noop;
	
	[[no_unique_address]] Allocator allocator = Allocator::current();
	v2u cursor = {};
	u32 row_height = {};
	u32 cursor_min_x = {};
	
	// stride_in_bytes - if 0, replaced with `new_elem_size.x * sizeof(Element)`
	Area &add(Element *new_elem, v2u new_elem_size, u32 stride_in_bytes = 0) {
		while (1) {
			if (cursor.x + new_elem_size.x <= size.x && cursor.y + new_elem_size.y <= size.y)
				break;

			cursor.x = cursor_min_x;
			cursor.y += row_height;
			row_height = 0;
			if (cursor.x + new_elem_size.x <= size.x && cursor.y + new_elem_size.y <= size.y)
				break;

			grow();
		}

		u32 &stride = stride_in_bytes;
		if (stride == 0)
			stride = new_elem_size.x * sizeof(Element);

		for (u32 y = 0; y < new_elem_size.y; ++y) {
			auto dst = &data[(cursor.y + y) * size.x + cursor.x];
			auto src = (u8 *)new_elem + y * stride;
			memcpy(dst, src, new_elem_size.x * sizeof(Element));
		}

		Area area = {};
		area.rect = {cursor, cursor + new_elem_size};
		auto &result = areas.add(area);
		
		on_modify(this, user_data, area.rect);

		cursor.x += new_elem_size.x;
		row_height = max(row_height, new_elem_size.y);

		return result;
	}

	void grow() {
		// Double size in one dimension, keep old stuff where it is,
		// new elements will be placed in new space

		auto old_data = data;
		auto old_size = size;

		if (size.x == 0) {
			cursor = {};
			row_height = 0;
			cursor_min_x = 0;

			size = {256, 256};
			data = allocator.allocate<Element>(size.x * size.y);
		} else {
			data = allocator.allocate<Element>(size.x * size.y * 2);

			if (size.x == size.y) {
				// Extend width. Left half is used, right is unused.
				// Stride is changed, move rows.
				// :performance: Consider fixed-width atlas that does not need row movement.

				for (u32 y = size.y - 1; y != -1; --y) {
					auto dst = &data[y * size.x * 2];
					auto src = &old_data[y * size.x];
					memcpy(dst, src, size.x * sizeof(Element));
				}

				cursor_min_x = size.x;

				size.x *= 2;
			} else {
				// Extend height. Top half is used, bottom is unused.
				// Stride is not changed.

				cursor = {0, size.y};
				row_height = 0;
				cursor_min_x = 0;

				size.y *= 2;
			}
		}
		
		for (u32 y = 0; y < old_size.y; ++y) {
			auto dst = &data[y * size.x];
			auto src = &old_data[y * old_size.x];
			memcpy(dst, src, old_size.x * sizeof(Element));
		}
		
		allocator.free(old_data);

		on_resize(this, user_data, size);
		on_modify(this, user_data, {{}, size});
	}

	void pack(bool keep_area_order = false) {
		auto old_data = allocator.allocate<Element>(size.x * size.y);
		defer { allocator.free(old_data); };

		memcpy(old_data, data, size.x * size.y * sizeof(Element));

		auto old_areas = copy(areas);
		defer { free(old_areas); };

		areas.clear();

		cursor = {};
		cursor_min_x = 0;
		row_height = 0;
		
		// Tallest first, then widest first
		quick_sort(old_areas, [](auto a, auto b) { 
			auto d = (v2s)b.rect.size() - (v2s)a.rect.size();
			if (d.y)
				return d.y;
			return d.x;
		});

		assert(!keep_area_order, "not implemented. TODO: keep area order as it was");
		for (auto old_area : old_areas) {
			auto elem = old_data[old_area.rect.min.y * size.x + old_area.rect.min.x];
			add(elem, old_area.rect.size());
		}
	}

	umm wasted_area() {
		umm allocated_area = cursor.y * size.x + cursor.x * row_height;
		
		umm wasted_area = allocated_area;
		for (auto area : areas) {
			auto s = area.rect.size();
			wasted_area -= s.x * s.y;
		}

		return wasted_area;
	}

	Element &operator[](v2u i) {
		bounds_check(assert(i.x < size.x));
		bounds_check(assert(i.y < size.y));
		return data[i.y * size.x + i.x];
	}

	void clear() {
		areas.clear();
		cursor = {};
		row_height = 0;
		cursor_min_x = 0;
	}
	void free() {
		allocator.free(data);
		data = 0;
		size = {};
		tl::free(areas);
		cursor = {};
		row_height = 0;
		cursor_min_x = 0;
	}
};

}
