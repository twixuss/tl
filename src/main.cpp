#include "../include/tl/tl.h"
#include "../include/tl/math_random.h"
using namespace TL;


#pragma warning(push, 0)
#include "math.cpp"
#include <stdio.h>
#include <assert.h>
#include <excpt.h>
#if COMPILER_GCC
#include <cxxabi.h>
#endif
#include <random>
#include <chrono>
#include <thread>
#include <typeinfo>

void string_test() {
	printf("%s ... ", "string_test");
	StringBuilder<char, OsAllocator, 16> builder;
	char const *str = "Surprise steepest recurred landlord mr wandered amounted of. Continuing devonshire but considered its. Rose past oh shew roof is song neat. Do depend better praise do friend garden an wonder to. Intention age nay otherwise but breakfast. Around garden beyond to extent by. ";
	builder.append(str);
	auto result = builder.get();
	assert(result.size() == strlen(str));
	assert(memcmp(result.data(), str, result.size()) == 0);

	puts("ok");
}

template <class T>
void bubbleSort(Span<T> span) {
	if (span.size() < 2)
		return;

	bool doSwap = true;

	while (doSwap) {
		doSwap = false;
		for (auto cursor = span.begin() + 1; 
			 cursor != span.end(); 
			 ++cursor)
		{
			if (cursor[0] < cursor[-1]) {
				std::swap(cursor[0], cursor[-1]);
				doSwap = true;
			}
		}
	}
}
// Fucking... nope, FUCKING FUCKING FUCKING FUCKING FUCKING c++!!!
template <template <class T, class ...Args> class List, class T, class ...Args> void bubbleSort(List<T, Args...> &list) { bubbleSort((Span<T>)list); }

template <class T, class Predicate>
void quickSort(Span<T> span, Predicate &&predicate) {
	if (span.size() < 2)
		return;

	auto partition = [&](T *begin, T *end) {
		//T mid = *midpoint(begin, end - 1);
		T mid = midpoint(begin[0], end[-1]);
		//T mid = (begin[0] + end[-1] + *midpoint(begin, end - 1)) / 3;
		--begin;
		for (;;) {
			while(predicate(*++begin, mid)) {}
			while(predicate(mid, *--end)) {}
			if (begin >= end)
				return end + 1;
			std::swap(*begin, *end);
		}
	};

	T *p = partition(span.begin(), span.end());
	quickSort(Span<T>{span.begin(), p}, predicate);
	quickSort(Span<T>{p, span.end()}, predicate);
}
template <class T>
void quickSort(Span<T> span) {
	quickSort(span, [](T &a, T &b) { return a < b; });
}

// Fucking... nope, FUCKING FUCKING FUCKING FUCKING FUCKING c++!!!
template <template <class T, class ...Args> class List, class T, class ...Args>
void quickSort(List<T, Args...> &list) {
	quickSort((Span<T>)list);
}
template <template <class T, class ...Args> class List, class T, class ...Args, class Predicate>
void quickSort(List<T, Args...> &list, Predicate &&predicate) {
	quickSort((Span<T>)list, predicate);
}

template<typename T>
void mergeSort(Span<T> span) {
	List<T> temp;
	temp.reserve(span.size());

	for (umm blockSize = 1; blockSize < span.size(); blockSize *= 2) {
		for (umm block = 0; block < span.size() - blockSize; block += 2 * blockSize) {
			umm left = 0;
			umm right = 0;
			umm begin = block;
			umm mid = block + blockSize;
			umm end = min(block + 2 * blockSize, span.size());
			umm size = end - begin;

			while (begin + left < mid && mid + right < end) {
				if (span[begin + left] < span[mid + right]) {
					temp.push_back(span[begin + left]);
					left += 1;
				} else {
					temp.push_back(span[mid + right]);
					right += 1;
				}
			}
			while (begin + left < mid) {
				temp.push_back(span[begin + left]);
				left += 1;
			}
			while (mid + right < end) {
				temp.push_back(span[mid + right]);
				right += 1;
			}
			for (umm i = 0; i < size; i++) {
				span[begin + i] = temp[i];
			}
			temp.clear();
		}
	}
}

#if 0

template<typename T>
void mergeSort(Span<T> span, Allocator al = osAllocator) {
	Span<T> temp {
		al.allocate<T>(span.size()),
		span.size()
	};
	defer { al.deallocate(temp.data()); }

	Span<T> *src = &span;
	Span<T> *dst = &temp;

	for (umm blockSize = 1; blockSize < src->size(); blockSize *= 2) {
		for (umm block = 0; block < src->size() - blockSize; block += 2 * blockSize) {
			umm left = 0;
			umm right = 0;
			umm begin = block;
			umm mid = block + blockSize;
			umm end = min(block + 2 * blockSize, src->size());
			umm size = end - begin;

			while (begin + left < mid && mid + right < end) {
				if (src->at(begin + left) < src->at(mid + right)) {
					dst->at(left + right) = std::move(src->at(begin + left));
					left += 1;
				} else {
					dst->at(left + right) = std::move(src->at(mid + right));
					right += 1;
				}
			}
			while (begin + left < mid) {
				dst->at(left + right) = std::move(src->at(begin + left));
				left += 1;
			}
			while (mid + right < end) {
				dst->at(left + right) = std::move(src->at(mid + right));
				right += 1;
			}
			//for (umm i = 0; i < size; i++) {
			//	src[begin + i] = dst[i];
			//}
			std::swap(src, dst);
		}
	}
	if (dst == &span) {
		for (umm i = 0; i < span.size(); i++) {
			span[i] = std::move(dst->at(i));
		}
	}
}

#endif

// Fucking... nope, FUCKING FUCKING FUCKING FUCKING FUCKING c++!!!
template <template <class T, class ...Args> class List, class T, class ...Args> void mergeSort(List<T, Args...> &list) { mergeSort((Span<T>)list); }

void radixSort(Span<u32> span) {
	List<u32> temp;
	temp.resize(span.size());

	Span<u32> src = span;
	Span<u32> dst = temp;

	u32 mask = 0xFF;
	u32 shift = 0;

	for (u32 r = 0; r < 4; ++r) {
		u32 counters[256]{};
		for (umm i = 0; i < src.size(); ++i) {
			++counters[(src[i] & mask) >> shift];
		}
		for (u32 i = 1; i < 256; ++i) {
			counters[i] += counters[i - 1];
		}
		for (auto it = src.end() - 1; it >= src.begin(); --it) {
			dst[--counters[(*it & mask) >> shift]] = *it;
		}
		mask <<= 8;
		shift += 8;
		std::swap(src, dst);
	}
}

struct Timer {
	char const *name;
	List<u32> *origList;
	std::thread thread;

	Timer(char const *name, List<u32> *origList) : origList(origList) {
		this->name = name;
	}
	Timer(const Timer &) = delete;
	Timer &operator=(const Timer &) = delete;

	template <class Fn>
	void operator+=(Fn &&fn) {
		thread = std::thread([&] {
			StringBuilder<> builder;
			builder.appendFormat("{}:\n", name);
			for (u32 size = 2; size <= origList->size(); size *= 2) {
				List<u32> origList2;
				origList2.resize(size);
				memcpy(origList2.data(), origList->data(), size * sizeof(origList2[0]));

				u64 minTime = (u64)~0;
				for (u32 i = 0; i < 0x100; ++i) {
					List<u32> list = origList2;
					auto begin = std::chrono::high_resolution_clock::now();
					fn(list);
					minTime = min(minTime, (u64)(std::chrono::high_resolution_clock::now() - begin).count());
				}
				if (minTime)
					builder.appendFormat("({},{}),", log2(size), log2(minTime));
				else
					builder.appendFormat("({},0),", log2(size));
			}
			TL::print(builder.get());
		});
	}
};

#define TIMER(name) Timer CONCAT(_timer_, __LINE__)(name, &origList); defer { CONCAT(_timer_, __LINE__).thread.join(); }; CONCAT(_timer_, __LINE__) += [&](auto &list)

void sort_test() {
	List<u32> origList;
	origList.resize(0x1000);
	u32 const scale = (u32)(0x100000000 / origList.size());
	for (u32 i = 0; i < origList.size(); ++i)
		origList[i] = i * scale;
	std::shuffle(origList.begin(), origList.end(), std::mt19937{std::random_device{}()});

	List<u32> list;
	
#define SORT_TEST(name)						  \
	{										  \
		printf("%s ... ", #name);             \
		list = origList;				      \
		name(list);                           \
		for (u32 i = 0; i < list.size(); ++i) \
			assert(list[i] == i * scale);	  \
		puts("ok");                           \
	}										  \

	SORT_TEST(bubbleSort);
	SORT_TEST(quickSort);
	SORT_TEST(mergeSort);
	SORT_TEST(radixSort);
}
void sort_perf() {
	List<u32> origList;
	origList.resize(0x100000);
	for (u32 i = 0; i < origList.size(); ++i)
		origList[i] = i;
	std::shuffle(origList.begin(), origList.end(), std::mt19937{std::random_device{}()});
	
	TIMER("std::sort") { std::sort(list.begin(), list.end()); };
	TIMER("quickSort") { quickSort(list); };
	TIMER("mergeSort") { mergeSort(list); };
}

#define memequ(a, b, s) (memcmp(a, b, s) == 0)

void buffer_test() {
	{
		CircularBuffer<char> buffer(8);
		assert(buffer.empty());

		buffer.push_back('a');
		buffer.push_back('b');
		buffer.push_back('c');
		buffer.push_back('d');
	
		assert(!buffer.empty());
		assert(!buffer.full());
		assert(memequ(buffer._allocBegin, "abcd", 4));
		assert(buffer.back() == 'd');
		assert(buffer.front() == 'a');

		buffer.push_front('e');
		buffer.push_front('f');
		buffer.push_front('g');
		buffer.push_front('h');
	
		assert(!buffer.empty());
		assert(buffer.full());
		assert(memequ(buffer._allocBegin, "abcdhgfe", 8));
		assert(buffer.back() == 'd');
		assert(buffer.front() == 'h');

		buffer.erase(buffer.begin());
		assert(buffer.size() == 7);
		assert(buffer.front() == 'g');

		buffer.erase(buffer.end() - 1);
		assert(buffer.size() == 6);
		assert(buffer.back() == 'c');
	}
	{
		CircularBuffer<char> buffer;

		buffer.push_back('a');
		assert(buffer.capacity() == 1);
		assert(memequ(buffer._allocBegin, "a", 1));

		buffer.push_back('b');
		assert(buffer.capacity() == 2);
		assert(memequ(buffer._allocBegin, "ab", 2));

		buffer.push_back('c');
		assert(buffer.capacity() == 4);
		assert(memequ(buffer._allocBegin, "abc", 3));

		buffer.push_front('d');
		assert(buffer.capacity() == 4);
		assert(memequ(buffer._allocBegin, "abcd", 4));

		buffer.push_front('e');
		assert(buffer.capacity() == 8);
		assert(memequ(buffer._allocBegin, "dabc", 4));
		assert(buffer._allocBegin[7] == 'e');
	}
}

f32 result__;

int main() {
	result__ = voronoi(v2f{.1f, .2f});

	buffer_test();
	sort_test();
	//sort_perf();

	string_test();

	math_test();
}
#pragma warning(pop)
