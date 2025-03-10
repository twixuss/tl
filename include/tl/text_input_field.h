#pragma once
#include "list.h"
#include "variant.h"
#include "hash_map.h"
#include "hash_set.h"

namespace tl {

struct TextInputField {
	struct Insert {
		List<utf32> inserted_text;
		List<utf32> erased_text;
		u32 selection_start_before = 0;
		u32 cursor_before = 0;
		u32 cursor_after = 0;
		bool mergeable = false;
	};
	struct Erase {
		List<utf32> text;
		u32 selection_start_before = 0;
		u32 cursor_before = 0;
		u32 cursor_after = 0;
	};
	using Action = Variant<Insert, Erase>;

	List<utf32> text;
	
	u32 selection_start = 0;
	u32 cursor = 0;

	List<Action> actions;
	u32 undoable_actions_count = 0;

	// If inserted text contains a banned char, it is replaced.
	// If replacement is zero, it is not inserted.
	// For example you might ban new lines, but replace tabs with spaces.
	HashMap<utf32, utf32> banned_chars;

	// If empty, all chars are allowed.
	HashSet<utf32> allowed_chars;

	// Temporary reusable storage.
	List<utf32> sanitized_string;

	void set_cursor(u32 cursor, bool keep_selection = false) {
		this->cursor = cursor;
		if (!keep_selection)
			selection_start = cursor;
	}
	
	u32 selection_size() {
		return absolute((s32)selection_start - (s32)cursor);
	}
	u32 selection_min() {
		return min(selection_start, cursor);
	}
	u32 selection_max() {
		return max(selection_start, cursor);
	}

	Span<utf32> selection() {
		auto begin = selection_start;
		auto end   = cursor;
		sort_values(begin, end);
		return Span(text.data + begin, end - begin);
	}
	Span<utf32> selection_or_all() {
		if (cursor == selection_start) {
			return text;
		} else {
			return selection();
		}
	}

	void select_all() {
		selection_start = 0;
		cursor = text.count;
	}

	void sanitize(Span<utf32> &string) {
		sanitized_string.set(string);

		if (banned_chars.count) {
			sanitized_string.here_map([&](utf32 c) -> Optional<utf32> {
				if (auto found = banned_chars.find(c)) {
					if (*found.value) {
						return *found.value;
					} 
					return {};
				}
				return c;
			});
		}

		if (allowed_chars.count) {
			sanitized_string.erase_all([&](utf32 c) { return !allowed_chars.find(c); });
		}

		string = sanitized_string;
	}

	void add_action(Action action) {
		if (actions.count) {
			auto old_insert = actions.back().as_ptr<Insert>();
			auto new_insert = action.as_ptr<Insert>();

			if (old_insert && new_insert && old_insert->mergeable && new_insert->mergeable) {
				if (old_insert->cursor_after == new_insert->cursor_before) {
					old_insert->inserted_text.add(new_insert->inserted_text);
					old_insert->cursor_after = new_insert->cursor_after;
					free(new_insert->inserted_text);
					return;
				}
			}
		}
		if (actions.count > undoable_actions_count) {
			for (auto action : actions.skip(undoable_actions_count)) {
				switch (action.index()) {
					case action.index_of<Insert>:
						free(action.as_ptr<Insert>()->inserted_text);
						free(action.as_ptr<Insert>()->erased_text);
						break;
					case action.index_of<Erase>:
						free(action.as_ptr<Erase>()->text);
						break;
				}
			}
			actions.count = undoable_actions_count;
		}
		actions.add(action);
		undoable_actions_count += 1;
	}

	void insert(Span<utf32> string, bool mergeable = false) {
		sanitize(string);

		if (!string.count)
			return;

		add_action(Insert{
			.inserted_text = to_list(string), 
			.erased_text = to_list(selection()),
			.selection_start_before = selection_start,
			.cursor_before = cursor,
			.cursor_after = selection_min() + (u32)string.count,
			.mergeable = mergeable && cursor >= selection_start,
		});
		text.replace(selection(), string);
		set_cursor(selection_min() + string.count);
	}

	void erase(Span<utf32> string) {
		// Bounds checks are performed in text.erase(string), no need to repeat them.
		u32 string_start = string.data - text.data;
		add_action(Erase{
			.text = to_list(string),
			.selection_start_before = selection_start,
			.cursor_before = cursor,
			.cursor_after = string_start,
		});

		text.erase(string);
		
		set_cursor(string_start);
	}
	void erase_all() {
		add_action(Erase{
			.text = to_list(text),
			.selection_start_before = selection_start,
			.cursor_before = cursor,
			.cursor_after = 0,
		});

		text.clear();
		
		set_cursor(0);
	}
	void erase_selection() {
		add_action(Erase{
			.text = to_list(selection()),
			.selection_start_before = selection_start,
			.cursor_before = cursor,
			.cursor_after = selection_min(),
		});

		text.erase(selection());
		
		set_cursor(selection_min());
	}
	void erase_char_forward() {
		if (selection_size() == 0) {

			if (cursor == text.count)
				return;
	
			add_action(Erase{
				.text = to_list(Span(&text[cursor], (umm)1)),
				.selection_start_before = cursor,
				.cursor_before = cursor,
				.cursor_after = cursor,
			});
		
			text.erase_at(cursor);
		} else {
			erase_selection();
		}
	}
	void erase_char_back() {
		if (selection_size() == 0) {
			if (cursor == 0) 
				return;

			add_action(Erase{
				.text = to_list(Span(&text[cursor - 1], (umm)1)),
				.selection_start_before = cursor,
				.cursor_before = cursor,
				.cursor_after = cursor - 1,
			});
			text.erase_at(cursor - 1);
			cursor -= 1;
			selection_start = cursor;
		} else {
			erase_selection();
		}
	}
	void erase_word_back() {
		if (cursor <= selection_start) {
			u32 original_cursor = cursor;

			move_word_back(true);

			if (selection_start == cursor)
				return;

			auto to_erase = Span(text.data + cursor, text.data + selection_start);
			add_action(Erase{
				.text = to_list(to_erase), 
				.selection_start_before = selection_start,
				.cursor_before = original_cursor, 
				.cursor_after = cursor,
			});
			text.erase(to_erase);

			selection_start = cursor;
		} else {
			erase_selection();
		}
	}
	void erase_word_forward() {
		if (cursor >= selection_start) {
			u32 original_cursor = cursor;
		
			move_word_forward(true);
		
			if (selection_start == cursor)
				return;

			auto to_erase = Span(text.data + selection_start, text.data + cursor);
			add_action(Erase{
				.text = to_list(to_erase),
				.selection_start_before = selection_start,
				.cursor_before = original_cursor,
				.cursor_after = selection_min(),
			});
			text.erase(to_erase);

			cursor = selection_start;
		} else {
			erase_selection();
		}
	}

	void set_text(Span<utf32> string) {
		sanitize(string);

		add_action(Insert{
			.inserted_text = to_list(string),
			.erased_text = to_list(text),
			.selection_start_before = selection_start,
			.cursor_before = cursor,
			.cursor_after = (u32)string.count,
		});

		text.set(string);
	
		set_cursor(text.count);
	}

	void move_char_back(bool keep_selection) {
		if (selection_min() != 0)
			cursor = selection_min() - 1;

		if (!keep_selection)
			selection_start = cursor;
	}
	void move_char_forward(bool keep_selection) {
		if (selection_max() != text.count)
			cursor = selection_max() + 1;
		
		if (!keep_selection)
			selection_start = cursor;
	}
	void move_word_back(bool keep_selection) {
		if (cursor != 0) {
			if (is_whitespace(text[cursor - 1])) {
				while (1) {
					if (cursor - 1 > text.count)
						break;
					--cursor;
					if (!is_whitespace(text[cursor])) {
						++cursor;
						break;
					}
				}
			}

			while (1) {
				if (cursor - 1 > text.count)
					break;
				--cursor;
				if (is_whitespace(text[cursor])) {
					++cursor;
					break;
				}
			}
		}
		
		if (!keep_selection)
			selection_start = cursor;
	}
	void move_word_forward(bool keep_selection) {
		if (cursor != text.count) {
			if (!is_whitespace(text[cursor])) {
				while (1) {
					++cursor;
					if (cursor >= text.count)
						break;
					if (is_whitespace(text[cursor]))
						break;
				}
			}
			
			if (cursor < text.count) {
				while (1) {
					++cursor;
					if (cursor >= text.count)
						break;
					if (!is_whitespace(text[cursor]))
						break;
				}
			}
		}
		
		if (!keep_selection)
			selection_start = cursor;
	}

	Span<utf32> inserted_text(Insert insert) {
		return Span(text.data + insert.cursor_after - insert.inserted_text.count, insert.inserted_text.count);
	}
	Span<utf32> erased_text(Insert insert) {
		return Span(text.data + insert.cursor_after - insert.inserted_text.count, insert.erased_text.count);
	}

	void undo() {
		if (undoable_actions_count == 0)
			return;
		undoable_actions_count -= 1;
		auto action = actions[undoable_actions_count];
		action.visit(Combine{
			[&](Insert insert) {
				text.replace(inserted_text(insert), insert.erased_text);
				selection_start = insert.selection_start_before;
				cursor = insert.cursor_before;
			},
			[&](Erase erase) {
				text.insert_at(erase.text, erase.cursor_after);
				selection_start = erase.selection_start_before;
				cursor = erase.cursor_before;
			},
		});
	}
	void redo() {
		if (undoable_actions_count == actions.count)
			return;
		auto action = actions[undoable_actions_count];
		undoable_actions_count += 1;
		action.visit(Combine{
			[&](Insert insert) {
				text.replace(erased_text(insert), insert.inserted_text);
				selection_start = insert.cursor_after;
				cursor = insert.cursor_after;
			},
			[&](Erase erase) {
				text.erase(text.subspan(erase.cursor_after, erase.text.count));
				selection_start = erase.cursor_after;
				cursor = erase.cursor_after;
			},
		});
	}
};

}