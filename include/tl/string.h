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
	using Base = List;
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
	void append(Span<char const> str) {
		auto requiredSize = size() + str.size();
		if (requiredSize > capacity()) {
			_grow(requiredSize);
		}
		memcpy(_end, str.data(), str.size());
		_end += str.size();
	}
	void append(Char const *str) {
		append(Span<char const>(str, length(str)));
	}
	s32 compare(String const &b) const {
		s32 result = 0;
        for (umm i = 0; i < min(size(), b.size()); ++i) {
            if ((*this)[i] != b[i]) {
                return (*this)[i] < b[i] ? -1 : +1;
            }
        }

		if (result)
			return result;
		if (size() < b.size())
			return -1;
		if (size() > b.size())
			return 1;
		return 0;
	}
};

template <class Char, class Allocator, class T>
String<Char, Allocator> toString(T const &val) {
	String<Char, Allocator> result;
	toString<Char>(val, [&](Char const *src, umm length) {
		result.resize(length);
		memcpy(result.data(), src, length * sizeof(Char));
		return Span<Char>();
	}); 
	return result;
}

template <class Char, class Allocator, class T>
String<Char, Allocator> toStringNT(T const &val) {
	String<Char, Allocator> result;
	toString<Char>(val, [&](Char const *src, umm length) {
		result.resize(length + 1);
		memcpy(result.data(), src, length * sizeof(Char));
		result[length] = (Char)0;
		return Span<Char>();
	}); 
	return result;
}

template <class Allocator = TL_DEFAULT_ALLOCATOR>
String<char, Allocator> nullTerminate(Span<char const> span) {
	String<Allocator> result;
	result.resize(span.size() + 1);
	memcpy(result.data(), span.data(), span.size() * sizeof(char));
	result[span.size()] = '\0';
	return result;
}

template <class Allocator = TL_DEFAULT_ALLOCATOR>
String<wchar, Allocator> nullTerminate(Span<wchar const> span) {
	String<wchar, Allocator> result;
	result.resize(span.size() + 1);
	memcpy(result.data(), span.data(), span.size() * sizeof(wchar));
	result[span.size()] = L'\0';
	return result;
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

template <class Char = char, class Allocator = TL_DEFAULT_ALLOCATOR, umm blockSize_ = 4096>
struct StringBuilder {
	using String = String<Char, Allocator>;

	static constexpr umm blockSize = blockSize_;
	struct Block {
		Char buffer[blockSize];
		umm size = 0;
		Block *next = 0;

		Block() {}
		Char *end() { return buffer + size; }
		umm availableSpace() { return blockSize - size; }
	};
	Block first = {};
	Block *last = &first;
	Block *allocLast = &first;

	StringBuilder() = default;
	StringBuilder(StringBuilder const &that) = delete;
	StringBuilder(StringBuilder&& that) {
		first = that.first;
		if (that.last == &that.first) last = &first;
		else last = that.last;
		
		if (that.allocLast == &that.first) allocLast = &first;
		else allocLast = that.allocLast;

		that.first.size = 0;
		that.first.next = 0;
		that.last = &that.first;
		that.allocLast = &that.first;
	}
	StringBuilder &operator=(StringBuilder const &that) = delete;
	StringBuilder &operator=(StringBuilder &&that) = delete;
	~StringBuilder() {
		Block *next = 0;
		for (Block *block = first.next; block != 0; block = next) {
			next = block->next;
			DEALLOCATE(Allocator, block);
		}
	}
	umm availableSpace() {
		umm space = 0;
		Block *block = last;
		while (block) {
			space += block->availableSpace();
			block = block->next;
		}
		return space;
	}
	void ensureSpace(umm amount) {
		umm space = availableSpace();
		while (space < amount) {
			Block *newBlock = allocateBlock();
			allocLast->next = newBlock;
			allocLast = newBlock;
			space += blockSize;
		}
	}
	void ensureConsecutiveSpace(umm amount) {
		ASSERT(amount <= blockSize);
		if (last->availableSpace() < amount) {
			if (last->next) {
				last = last->next;
				ASSERT(last->availableSpace() == blockSize); // @DEBUG
			} else {
				last->next = allocateBlock();
				last = allocLast = last->next;
			}
		}
	}
	umm append(Span<Char const> span) {
		umm charsWritten = span.size();
		umm charsToWrite = span.size();
		while (last->availableSpace() < charsToWrite) {
			umm spaceInBlock = last->availableSpace();
			memcpy(last->end(), span.data(), spaceInBlock * sizeof(Char));
			charsToWrite -= spaceInBlock;
			last->size += spaceInBlock;
			span = {span.begin() + spaceInBlock, span.end()};
			if (!last->next) {
				last->next = allocLast = allocateBlock();
				last = last->next;
			}
		}
		memcpy(last->end(), span.data(), charsToWrite * sizeof(Char));
		last->size += charsToWrite;
		return charsWritten;
	}
	umm append(Char const *str) { return append(Span{str ? str : "(null)", str ? length(str) : 0}); }
	umm append(Char ch, umm count = 1) {
		umm charsToWrite = count;
		while (last->availableSpace() < charsToWrite) {
			umm spaceInBlock = last->availableSpace();
			populate(last->end(), ch, spaceInBlock);
			charsToWrite -= spaceInBlock;
			last->size += spaceInBlock;
			last = last->next;
			if (!last) {
				last = allocLast = allocateBlock();
			}
		}
		populate(last->end(), ch, charsToWrite);
		last->size += charsToWrite;
		return count;
	}
	template <class T>
	umm append(T const &val) {
		return toString<Char>(val, [&] (Char const *src, umm length) {
			return append(Span<Char const>(src, length));
		});
	}
	umm appendBytes(void const *address, umm size) {
		append(Span((Char *)address, (Char *)address + size));
		return size;
	}
	template <class T>
	umm appendBytes(T const &value) {
		return appendBytes(std::addressof(value), sizeof(value));
	}
	umm appendFormat(Char const *fmt) {
		return append(Span{fmt, length(fmt)});
	}
	template <class Arg, class ...Args>
	umm appendFormat(Char const *fmt, Arg const &arg, Args const &...args) {
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
		//ASSERT(fmtBegin && fmtEnd, "invalid format");
		if (!(fmtBegin && fmtEnd)) {
			return appendFormat(fmt);
		}

		umm charsAppended = append(Span{fmt, fmtBegin});

		if (argAsFormat) {
			if constexpr (isSame<RemoveReference<std::decay_t<Arg>>, Char const *> || isSame<RemoveReference<std::decay_t<Arg>>, Char *>)
				charsAppended += appendFormat(arg, args...);
			else 
				INVALID_CODE_PATH("'%fmt%' arg is not a string");
		} else {
			toString<Char>(arg, [&] (Char const *src, umm length) {
				charsAppended += length;
				append(Span<Char const>(src, length));
				return Span<Char>();
			});
		}

		charsAppended += appendFormat(fmtEnd, args...);
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
		ASSERT(dstString.size() >= this->size());
		Char *dstChar = dstString.data();
		for (Block *block = &first; block != 0; block = block->next) {
			memcpy(dstChar, block->buffer, block->size * sizeof(Char));
			dstChar += block->size;
		}
		return Span<Char>(dstString.begin(), dstChar);
	}
	String get() {
		String result(size());
		fill(result);
		return result;
	}
	String getNullTerminated() {
		String result(size() + 1);
		fill(result);
		result.back() = '\0';
		return result;
	}

	template <class Fn>
	void stream(Fn &&fn) {
		Block *block = &first;
		do {
			auto blockSize = block->size;
			if (blockSize) {
				fn(block->buffer, blockSize);
			}
			block = block->next;
		} while (block);
	}

private:
	Block *allocateBlock() { return construct(ALLOCATE_T(Allocator, Block, 1, 0)); }
};

template <class Char, class Allocator = TL_DEFAULT_ALLOCATOR, class ...Args>
String<Char, Allocator> format(Char const *fmt, Args const &...args) {
	StringBuilder<Char, Allocator> builder;
	builder.appendFormat(fmt, args...);
	return builder.get();
}

template <class Char, class Allocator = TL_DEFAULT_ALLOCATOR, class ...Args>
String<Char, Allocator> formatAndTerminate(Char const *fmt, Args const &...args) {
	StringBuilder<Char, Allocator> builder;
	builder.appendFormat(fmt, args...);
	builder.append((Char)0);
	return builder.get();
}

}

#pragma warning(pop)
