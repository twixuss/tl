#pragma once
#include "common.h"
#include "list.h"
#include "optional.h"

#pragma warning(push)
#pragma warning(disable: 4820)

namespace TL {

template <class Allocator = TL_DEFAULT_ALLOCATOR>
inline wchar *copyString(wchar const *str) {
	umm size = (length(str) + 1) * sizeof(wchar);
	wchar *result = ALLOCATE_T(Allocator, wchar, size, 0);
	memcpy(result, str, size);
	return result;
}

template <class Char = char, class Allocator = TL_DEFAULT_ALLOCATOR>
struct String : List<Char, Allocator> {
	using Base = List<Char, Allocator>;
	using Base::operator[];
	String() = default;
	explicit String(umm size) : Base(size) {}
	String(Span<Char const> span) : Base(span) {}
	String(String const &that) = default;
	String(String &&that) = default;
	String(Char const *str, umm length) : Base(str, length) {}
	String(Char const *str) : Base(str, length(str)) {}
	String &operator=(String const &that) = default;
	String &operator=(String &&that) = default;
	String &operator=(Char const *str) {
		set(Span<char const>(str, length(str)));
		return *this;
	}
	String &set(Span<Char const> span) { return Base::set(span), *this; }
	void append(Span<Char const> str) { *this += str; }
	void append(Char const *str) {
		append(Span<char const>(str, length(str)));
	}
	s32 compare(String const &b) const {
		s32 result = 0;
        for (umm i = 0; i < min(this->size(), b.size()); ++i) {
            if ((*this)[i] != b[i]) {
                return (*this)[i] < b[i] ? -1 : +1;
            }
        }

		if (result)
			return result;
		if (this->size() < b.size())
			return -1;
		if (this->size() > b.size())
			return 1;
		return 0;
	}
	bool operator==(char const *that) const {
		return as_span(*this) == as_span(that);
	}
};

template <class Allocator = TL_DEFAULT_ALLOCATOR, class ...Args>
constexpr String<char, Allocator> concatenate(Args &&...args) {
	String<char, Allocator> result;
	int x[] = {0, (result.append(as_span(std::forward<Args>(args))), 0)...};
	return result;
}

template <class Allocator = TL_DEFAULT_ALLOCATOR, class Char = char, class T>
String<Char, Allocator> asString(T const &val) {
	String<Char, Allocator> result;
	to_string<Char>(val, [&](Span<Char const> span) {
		result = span;
	}); 
	return result;
}

template <class Allocator = TL_DEFAULT_ALLOCATOR, class Char = char, class T>
String<Char, Allocator> asStringNT(T const &val) {
	String<Char, Allocator> result;
	to_string<Char>(val, [&](Span<Char const> span) {
		result.resize(span.size() + 1);
		memcpy(result.data(), span.data(), span.size() * sizeof(Char));
		result[span.size()] = (Char)0;
	}); 
	return result;
}

template <class Allocator = TL_DEFAULT_ALLOCATOR, class Char>
String<Char, Allocator> null_terminate(Span<Char const> span) {
	return asStringNT<Allocator, Char>(span);
}
template <class Allocator = TL_DEFAULT_ALLOCATOR, class Char>
String<Char, Allocator> null_terminate(Span<Char> span) {
	return null_terminate<Allocator, Char>((Span<Char const>)span);
}

template <class T>
struct ParseResult {
	T value;
	bool success;
};

inline ParseResult<s64> parseDecimal(Span<char const> s) {
	ParseResult<s64> result = {};
	s64 oldResult = 0;
	bool negative = false;
	if (s.front() == '-') {
		negative = true;
		s._begin++;
	}
	for (auto c : s) {
		result.value *= 10;
		s64 digit = (s64)c - '0';
		if ((u64)digit > 9)
			return {};
		result.value += digit;
		if (result.value < oldResult)
			return {};
		oldResult = result.value;
	}
	if (negative) {
		result.value = -result.value;
	}
	result.success = true;
	return result;
}

inline ParseResult<f64> parseDecimalFloat(Span<char const> s) {
	ParseResult<f64> result = {};
	bool negative = false;
	if (s.front() == '-') {
		negative = true;
		s._begin++;
	}
	bool dot = false;
	u32 placeAfterDot = 0;
	for (auto c : s) {
		if (c == '.') {
			dot = true;
			continue;
		}
		if (dot) {
			s64 digit = (s64)c - '0';
			if ((u64)digit > 9)
				return {};
			result.value += (f64)digit / pow(10, placeAfterDot++);
		} else {
			result.value *= 10;
			s64 digit = (s64)c - '0';
			if ((u64)digit > 9)
				return {};
			result.value += (f64)digit;
		}
	}
	if (negative) {
		result.value = -result.value;
	}
	result.success = true;
	return result;
}

template <class Char, umm block_size_ = 4096>
struct StringBuilder {
	static constexpr umm block_size = block_size_;
	struct Block {
		Char buffer[block_size];
		umm size = 0;
		Block *next = 0;

		Block() {}
		Char *end() { return buffer + size; }
		umm available_space() { return block_size - size; }
	};

	Allocator allocator;
	Block first = {};
	Block *last = &first;
	Block *alloc_last = &first;

	StringBuilder() = default;
	StringBuilder(StringBuilder const &that) = delete;
	StringBuilder(StringBuilder&& that) = default;
	StringBuilder &operator=(StringBuilder const &that) = delete;
	StringBuilder &operator=(StringBuilder &&that) = default;
	~StringBuilder() {
		for (Block *block = first.next; block != 0;) {
			Block *next = block->next;
			free(allocator, block);
			block = next;
		}
	}
	umm available_space() {
		umm space = 0;
		Block *block = last;
		while (block) {
			space += block->available_space();
			block = block->next;
		}
		return space;
	}
	void ensureSpace(umm amount) {
		umm space = available_space();
		while (space < amount) {
			Block *new_block = allocate_block();
			alloc_last->next = new_block;
			alloc_last = new_block;
			space += block_size;
		}
	}
	void ensure_consecutive_space(umm amount) {
		assert(amount <= block_size);
		if (last->available_space() < amount) {
			if (last->next) {
				last = last->next;
			} else {
				last->next = allocate_block();
				last = alloc_last = last->next;
			}
		}
	}
	umm append(Span<char const> span) {
		umm chars_written = span.size();
		umm chars_to_write = span.size();
		while (last->available_space() < chars_to_write) {
			umm space_in_block = last->available_space();
			memcpy(last->end(), span.data(), space_in_block * sizeof(Char));
			chars_to_write -= space_in_block;
			last->size += space_in_block;
			span = {span.begin() + space_in_block, span.end()};
			if (!last->next) {
				last->next = alloc_last = allocate_block();
				last = last->next;
			}
		}
		memcpy(last->end(), span.data(), chars_to_write * sizeof(Char));
		last->size += chars_to_write;
		return chars_written;
	}
	umm append(char const *str) { return append(as_span(str)); }
	umm append(char ch, umm count = 1) {
		umm chars_to_write = count;
		while (last->available_space() < chars_to_write) {
			umm space_in_block = last->available_space();
			populate(last->end(), ch, space_in_block);
			chars_to_write -= space_in_block;
			last->size += space_in_block;
			last = last->next;
			if (!last) {
				last = alloc_last = allocate_block();
			}
		}
		populate(last->end(), ch, chars_to_write);
		last->size += chars_to_write;
		return count;
	}
	template <class T>
	umm append(T const &val) {
		return to_string<Char>(val, [&](Span<Char const> span) {
			return append(span);
		});
	}
	umm append_bytes(void const *address, umm size) {
		append(Span((Char *)address, (Char *)address + size));
		return size;
	}
	template <class T>
	umm append_bytes(T const &value) {
		return append_bytes(std::addressof(value), sizeof(value));
	}
	umm append_format(char const *fmt) {
		if (!*fmt)
			return 0;
		char const *c = fmt;
		while (*c) {
			if (*c == '%') {
				++c;
				if (*c != '`') {
					INVALID_CODE_PATH("bad format");
				}
				append(Span{fmt, c});
				++c;
				fmt = c;
			} else {
				++c;
			}
		}
		return append(Span{fmt, length(fmt)});
	}
	template <class Arg, class ...Args>
	umm append_format(Char const *fmt, Arg const &arg, Args const &...args) {
		if (!*fmt)
			return 0;
		Char const *c = fmt;
		Char const *fmtBegin = 0;
		Char const *fmtEnd = 0;

		bool argAsFormat = false;
		
		while (*c) {
			if (*c == '%') {
				fmtBegin = c;
				++c;
				if (*c == '`') {
					append(Span{fmt, c});
					++c;
					fmt = c;
					fmtBegin = 0;
					continue;
				}
				if (startsWith(c, "fmt%")) {
					argAsFormat = true;
					c += 4;
				}
				fmtEnd = c;
				break;
			}
			++c;
		}
		//assert(fmtBegin && fmtEnd, "invalid format");
		if (!(fmtBegin && fmtEnd)) {
			return append_format(fmt);
		}

		umm charsAppended = append(Span{fmt, fmtBegin});

		if (argAsFormat) {
			if constexpr (isSame<RemoveReference<std::decay_t<Arg>>, Char const *> || isSame<RemoveReference<std::decay_t<Arg>>, Char *>)
				charsAppended += append_format(arg, args...);
			else 
				INVALID_CODE_PATH("'%fmt%' arg is not a string");
		} else {
			to_string<Char>(arg, [&] (Span<Char const> span) {
				charsAppended += span.size();
				append(span);
			});
		}

		charsAppended += append_format(fmtEnd, args...);
		return charsAppended;
	}
	umm size() {
		umm totalSize = 0;
		for (Block *block = &first; block != 0; block = block->next) {
			totalSize += block->size;
		}
		return totalSize;
	}
	Span<Char> fill(Span<Char> dstString) {
		assert(dstString.size() >= this->size());
		Char *dstChar = dstString.data();
		for (Block *block = &first; block != 0; block = block->next) {
			memcpy(dstChar, block->buffer, block->size * sizeof(Char));
			dstChar += block->size;
		}
		return Span<Char>(dstString.begin(), dstChar);
	}
	template <class Char = char>
	String<Char> get() {
		String result(size());
		fill(result);
		return result;
	}
	template <class Char = char>
	String<Char> getNullTerminated() {
		String<Char> result(size() + 1);
		fill(result);
		result.back() = '\0';
		return result;
	}

	template <class Fn>
	void stream(Fn &&fn) {
		Block *block = &first;
		do {
			auto block_size = block->size;
			if (block_size) {
				fn(block->buffer, block_size);
			}
			block = block->next;
		} while (block);
	}
	void clear() {
		Block *block = &first;
		do {
			block->size = 0;
			block = block->next;
		} while (block);
		last = &first;
	}

private:
	Block *allocate_block() {
		return new (allocate<Block>(allocator)) Block;
	}
};

template <class Char = char, umm capacity = 4096>
StringBuilder<Char, capacity> create_string_builder() {
	StringBuilder<Char, capacity> result;
	result.allocator = current_allocator;
	return result;
}

template <class Allocator = TL_DEFAULT_ALLOCATOR, class Char, class ...Args>
String<Char, Allocator> format(Char const *fmt, Args const &...args) {
	StringBuilder<Char, Allocator> builder;
	builder.append_format(fmt, args...);
	return builder.get();
}

template <class Allocator = TL_DEFAULT_ALLOCATOR, class Char, class ...Args>
String<Char, Allocator> formatAndTerminate(Char const *fmt, Args const &...args) {
	StringBuilder<Char, Allocator> builder;
	builder.append_format(fmt, args...);
	builder.append((Char)0);
	return builder.get();
}

template <class Char, umm capacity, class T>
void append_string(StaticList<Char, capacity> &list, T const &value) {
	to_string<Char>(value, [&](Span<Char const> span) { list += span; });
}

}

#pragma warning(pop)
