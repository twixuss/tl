#pragma once
#include "common.h"
#include "list.h"
#include "optional.h"

namespace TL {

inline wchar *copyString(wchar const *str, Allocator allocator = osAllocator) {
	umm size = (length(str) + 1) * sizeof(wchar);
	wchar *result = (wchar *)allocator.allocate(size);
	memcpy(result, str, size);
	return result;
}

template <class Allocator = TL_DEFAULT_ALLOCATOR>
struct String : List<char, Allocator> {
	using Base = List;
	String() = default;
	explicit String(umm size) : Base(size) {}
	String(Span<char const> span) : Base(span) {}
	String(String const &that) = default;
	String(String &&that) = default;
	String(char const *str) : Base(str, strlen(str)) {}
	String &operator=(String const &that) = default;
	String &operator=(String &&that) = default;
	String &operator=(char const *str) {
		set(StringView(str, strlen(str)));
	}
	String &set(Span<char const> span) { return Base::set(span), *this; }
	void append(StringView str) {
		auto requiredSize = size() + str.size();
		if (requiredSize > capacity()) {
			_grow(requiredSize);
		}
		memcpy(_end, str.data(), str.size());
		_end += str.size();
	}
	void append(char const *str) {
		append(StringView(str, strlen(str)));
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

template <class Allocator, class T>
String<Allocator> toString(T const &val) {
	String<Allocator> result;
	toString([&](char const *src, umm length) {
		result.resize(length);
		memcpy(result.data(), src, length);
		return StringSpan();
	}, val); 
	return result;
}

template <class Allocator, class T>
String<Allocator> toStringNT(T const &val) {
	String<Allocator> result;
	toString([&](char const *src, umm length) {
		result.resize(length + 1);
		memcpy(result.data(), src, length);
		result[length] = '\0';
		return StringSpan();
	}, val); 
	return result;
}

template <class Allocator>
String<Allocator> nullTerminate(Span<char const> span) {
	String<Allocator> result;
	result.resize(span.size() + 1);
	memcpy(result.data(), span.data(), span.size());
	result[span.size()] = '\0';
	return result;
}

inline Optional<u64> parseDecimal(StringView s) {
	u64 result	  = 0;
	u64 oldResult = 0;
	for (auto c : s) {
		result *= 10;
		u64 digit = (u64)c - '0';
		if (digit > 9)
			return {};
		result += digit;
		if (result < oldResult)
			return {};
		oldResult = result;
	}
	return result;
}

template <class Allocator = TL_DEFAULT_ALLOCATOR, umm blockSize_ = 4096>
struct StringBuilder {
	using String = String<Allocator>;
	using Char = char;

	static constexpr umm blockSize = blockSize_;
	struct Block {
		Char buffer[blockSize];
		Char *end = buffer;
		Block *next = 0;

		Block() {}
		umm size() { return (umm)(end - buffer); }
		umm availableSpace() { return blockSize - size(); }
	};
	Block first;
	Block *last = &first;
	Block *allocLast = &first;

	StringBuilder() = default;
	StringBuilder(StringBuilder const &that) = delete;
	StringBuilder(StringBuilder &&that) = delete;
	StringBuilder &operator=(StringBuilder const &that) = delete;
	StringBuilder &operator=(StringBuilder &&that) = delete;
	~StringBuilder() {
		Block *next = 0;
		for (Block *block = first.next; block != 0; block = next) {
			next = block->next;
			Allocator::deallocate(block);
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
		umm charsToWrite = span.size();
		while (last->availableSpace() < charsToWrite) {
			umm spaceInBlock = last->availableSpace();
			memcpy(last->end, span.data(), spaceInBlock);
			charsToWrite -= spaceInBlock;
			last->end += spaceInBlock;
			span = {span.begin() + spaceInBlock, span.end()};
			last = last->next;
			if (!last) {
				last = allocLast = allocateBlock();
			}
		}
		memcpy(last->end, span.data(), charsToWrite);
		last->end += charsToWrite;
		return span.size();
	}
	umm append(Char const *str) { return append(Span{str ? str : "(null)", str ? strlen(str) : 0}); }
	umm append(Char ch, umm count = 1) {
		umm charsToWrite = count;
		while (last->availableSpace() < charsToWrite) {
			umm spaceInBlock = last->availableSpace();
			memset(last->end, ch, spaceInBlock);
			charsToWrite -= spaceInBlock;
			last->end += spaceInBlock;
			last = last->next;
			if (!last) {
				last = allocLast = allocateBlock();
			}
		}
		memset(last->end, ch, charsToWrite);
		last->end += charsToWrite;
		return count;
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
		return append(Span{fmt, strlen(fmt)});
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
			if (*c == '{') {
				fmtBegin = c;
				++c;
				if (*c == '{') {
					append(Span{fmt, c});
					++c;
					fmt = c;
					fmtBegin = 0;
					continue;
				}
				if (*c == '}') {
					c += 1;
				} else if (startsWith(c, "fmt}")) {
					argAsFormat = true;
					c += 4;
				} else {
					INVALID_CODE_PATH("bad format");
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
			if constexpr (isSame<RemoveReference<Arg>, Char const *> || isSame<RemoveReference<Arg>, Char *>)
				charsAppended += appendFormat(arg, args...);
			else 
				INVALID_CODE_PATH("'{fmt}' arg is not a string");
		} else {
			toString([&] (Char const *src, umm length) {
				charsAppended += length;
				append(Span{src, length});
				return StringSpan();
			}, arg);
		}

		charsAppended += appendFormat(fmtEnd, args...);
		return charsAppended;
	}
	umm size() {
		umm totalSize = 0;
		for (Block *block = &first; block != 0; block = block->next) {
			totalSize += block->size();
		}
		return totalSize;
	}
	StringSpan fill(StringSpan dstString) {
		ASSERT(dstString.size() >= this->size());
		Char *dstChar = dstString.data();
		for (Block *block = &first; block != 0; block = block->next) {
			memcpy(dstChar, block->buffer, block->size());
			dstChar += block->size();
		}
		return StringSpan(dstString.begin(), dstChar);
	}
	String get(bool terminate = false) {
		String result(size() + terminate);
		fill(result);
		if (terminate)
			result.back() = '\0';
		return result;
	}
	String getTerminated() { return get(true); }

	template <class Fn>
	void stream(Fn &&fn) {
		Block *block = &first;
		do {
			auto blockSize = block->size();
			if (blockSize) {
				fn(block->buffer, blockSize);
			}
			block = block->next;
		} while (block);
	}

private:
	Block *allocateBlock() { return construct(allocate<Block, Allocator>()); }
};

// --- Format options: ---						--- Example ---							--- Result ---
// {} - append an argument using 'toString'		format("{}", 5)							"5"
// {ar:N} - align right, N is width				format("{ar:3}", 5)						"  5"
// {al:N} - align left, N is width				format("{al:3}", 5)						"5  "
// {fmt} - append an argument as a format		format("{} {fmt}", 1, "{} {}", 2, 3)	"1 2 3"		NOTE: this option should be last in the format string
template <class Allocator = TL_DEFAULT_ALLOCATOR, class ...Args>
String<Allocator> format(char const *fmt, Args const &...args) {
	StringBuilder<Allocator> builder;
	builder.appendFormat(fmt, args...);
	return builder.get();
}

template <class Allocator = TL_DEFAULT_ALLOCATOR, class ...Args>
String<Allocator> formatAndTerminate(char const *fmt, Args const &...args) {
	StringBuilder<Allocator> builder;
	builder.appendFormat(fmt, args...);
	builder.append('\0');
	return builder.get();
}

}
