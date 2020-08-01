#pragma once
#include "common.h"
#include "list.h"
#include "optional.h"

namespace TL {

template <class Allocator = OsAllocator>
struct String : List<char, Allocator> {
	using Base = List;
	String() = default;
	explicit String(umm size) : Base(size) {}
	String(Span<char const> span) : Base(span) {}
	String(String const &that) = default;
	String(String &&that) = default;
	String &operator=(String const &that) = default;
	String &operator=(String &&that) = default;
	String &set(Span<char const> span) { return Base::set(span), *this; }
	String(char const *str) : Base(str, strlen(str)) {}

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

template <class Allocator, class ...Args>
String<Allocator> toString(Args const &...args) {
	String<Allocator> result;
	toString(args..., [&](char const *src, umm length) {
		result.resize(length);
		memcpy(result.data(), src, length);
		return StringSpan();
	}); 
	return result;
}

template <class Allocator, class ...Args>
String<Allocator> toStringNT(Args const &...args) {
	String<Allocator> result;
	toString(args..., [&](char const *src, umm length) {
		result.resize(length + 1);
		memcpy(result.data(), src, length);
		result[length] = '\0';
		return StringSpan();
	}); 
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

Optional<u64> parseDecimal(StringView s) {
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

template <class Allocator = OsAllocator, umm bufferSize = 4096>
struct StringBuilder {
	using String = String<Allocator>;
	struct Block {
		char buffer[bufferSize];
		char *end = buffer;
		Block *next = 0;

		Block() {}
		umm size() { return (umm)(end - buffer); }
		umm remaining() { return bufferSize - size(); }
	};
	Block first;
	Block *last = &first;

	StringBuilder() = default;
	StringBuilder(StringBuilder const &) = delete;
	StringBuilder(StringBuilder &&) = default;
	~StringBuilder() {
		Block *next = 0;
		for (Block *block = first.next; block != 0; block = next) {
			next = block->next;
			Allocator::deallocate(block);
		}
	}

	umm append(Span<char const> span) {
		umm charsToWrite = span.size();
		while (last->remaining() < charsToWrite) {
			umm remCharCount = last->remaining();
			memcpy(last->end, span.data(), remCharCount);
			charsToWrite -= remCharCount;
			last->end += remCharCount;
			span = {span.begin() + remCharCount, span.end()};

			Block *newBlock = construct(allocate<Block, Allocator>());
			last->next = newBlock;
			last = newBlock;
		}
		memcpy(last->end, span.data(), charsToWrite);
		last->end += charsToWrite;
		return span.size();
	}
	umm append(char const *str) { return append(Span{str ? str : "(null)", str ? strlen(str) : 0}); }
	umm append(char c) { return append(Span{&c, 1}); }
	
	umm appendFormat(char const *fmt) {
		return append(Span{fmt, strlen(fmt)});
	}
	template <class Arg, class ...Args>
	umm appendFormat(char const *fmt, Arg const &arg, Args const &...args) {
		if (!*fmt)
			return 0;
		char const *c = fmt;
		char const *fmtBegin = 0;
		char const *fmtEnd = 0;

		bool argAsFormat = false;
		
		bool usePrecision = false;
		u32 precision = 5;
		
		bool useAlign = false;
		u32 align = 0;
		bool alignRight = false;

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
					fmtEnd = c + 1;
					break;
				}
				auto parseU32 = [&] {
					char const *numStart = c;
					while (*c != '}' && *c != ',') {
						++c;
					}
					ASSERT(numStart != c, "invalid format: missing number after ':'");
					auto parsed = parseDecimal(StringView(numStart, c));
					ASSERT(parsed.has_value(), "invalid format: expected a number after ':'");
					ASSERT(parsed.value() <= 0xFFFF, "number too big");
					return (u32)parsed.value();
				};
				for (;;) { 
					if (startsWith(c, "fmt")) {
						ASSERT(!usePrecision, "'p' and 'fmt' cannot be used together");
						argAsFormat = true;
						c += 3;
					} else if (*c == 'p') {
						ASSERT(!argAsFormat, "'p' and 'fmt' cannot be used together");
						usePrecision = true;
						++c;
						ASSERT(*c == ':', "invalid format: missing ':' after align");
						++c;
						precision = parseU32();
					} else if (*c == 'a') {
						useAlign = true;
						++c;
						if (*c == 'r') {
							alignRight = true;
						} else if (*c == 'l') {
							alignRight = false;
						} else {
							INVALID_CODE_PATH("invalid format: bad align");
						}
						++c;
						ASSERT(*c == ':', "invalid format: missing ':' after align");
						++c;
						align = parseU32();
					} else {
						INVALID_CODE_PATH("invalid format");
					}
					if (*c == '}') {
						fmtEnd = c + 1;
						break;
					}
					ASSERT(*c == ',', "expected ',' after format option");
					++c;
				}
				break;
			}
			++c;
		}
		//ASSERT(fmtBegin && fmtEnd, "invalid format");
		if (!(fmtBegin && fmtEnd)) {
			return appendFormat(fmt);
		}

		umm charsAppended = append(Span{fmt, fmtBegin});

		auto appendArg = [&] (auto &builder) {
			umm result;
			if (argAsFormat) {
				if constexpr (isSame<RemoveReference<Arg>, char const *> || isSame<RemoveReference<Arg>, char *>)
					result = builder.appendFormat(arg, args...);
				else 
					INVALID_CODE_PATH("'{fmt}' arg is not a string");
			} else {
				if (usePrecision) {
					toString(arg, [&] (char const *src, umm length) {
						result = length;
						builder.append(Span{src, length});
						return StringSpan();
					}, Fmt::precision(precision));
				} else {
					toString(arg, [&] (char const *src, umm length) {
						result = length;
						builder.append(Span{src, length});
						return StringSpan();
					});
				}
			}
			charsAppended += result;
			return result;
		};
		
		if (align) {
			if (alignRight) {
				StringBuilder temp;
				umm argLen = appendArg(temp);
				if (align > argLen) {
					umm spaceCount = align - argLen;
					for (umm i = 0; i < spaceCount; ++i) {
						append(' ');
					}
					charsAppended += spaceCount;
				}
				append(temp.get());
			} else {
				umm argLen = appendArg(*this);
				if (align > argLen) {
					umm spaceCount = align - argLen;
					for (umm i = 0; i < spaceCount; ++i) {
						append(' ');
					}
					charsAppended += spaceCount;
				}
			}
		} else {
			appendArg(*this);
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
	StringSpan fill(StringSpan dst_) {
		ASSERT(dst_.size() >= this->size());
		char *dst = dst_.data();
		for (Block *block = &first; block != 0; block = block->next) {
			memcpy(dst, block->buffer, block->size());
			dst += block->size();
		}
		return StringSpan(dst_.begin(), dst);
	}
	String get(bool terminate = false) {
		String result(size() + terminate);
		fill(result);
		if (terminate)
			result.back() = '\0';
		return result;
	}
	String getTerminated() { return get(true); }
};

// --- Format options: ---						--- Example ---							--- Result ---
// {} - append an argument using 'toString'		format("{}", 5)							"5"
// {ar:N} - align right, N is width				format("{ar:3}", 5)						"  5"
// {al:N} - align left, N is width				format("{al:3}", 5)						"5  "
// {fmt} - append an argument as a format		format("{} {fmt}", 1, "{} {}", 2, 3)	"1 2 3"		NOTE: this option should be last in the format string
template <class Allocator = OsAllocator, class ...Args>
String<Allocator> format(char const *fmt, Args const &...args) {
	StringBuilder<Allocator> builder;
	builder.appendFormat(fmt, args...);
	return builder.get();
}

template <class Allocator = OsAllocator, class ...Args>
String<Allocator> formatAndTerminate(char const *fmt, Args const &...args) {
	StringBuilder<Allocator> builder;
	builder.appendFormat(fmt, args...);
	builder.append('\0');
	return builder.get();
}

}
