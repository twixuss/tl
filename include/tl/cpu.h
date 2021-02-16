#pragma once
#include "common.h"

#pragma warning(push)
#pragma warning(disable: 4061)

namespace TL {

enum class CpuFeature : u8 {
	_3dnow,
	_3dnowext,
	abm,
	adx,
	aes,
	avx,
	avx2,
	avx512cd,
	avx512er,
	avx512f,
	avx512pf,
	bmi1,
	bmi2,
	clfsh,
	cmov,
	cmpxchg16b,
	cx8,
	erms,
	f16c,
	fma,
	fsgsbase,
	fxsr,
	hle,
	invpcid,
	lahf,
	lzcnt,
	mmx,
	mmxext,
	monitor,
	movbe,
	msr,
	osxsave,
	pclmulqdq,
	popcnt,
	prefetchwt1,
	rdrand,
	rdseed,
	rdtscp,
	rtm,
	sep,
	sha,
	sse,
	sse2,
	sse3,
	sse41,
	sse42,
	sse4a,
	ssse3,
	syscall,
	tbm,
	xop,
	xsave,

	count,
};

enum class CpuVendor : u8 {
	unknown,
	intel,
	amd,

	count,
};

enum class CpuCacheType : u8 {
	unified, 
	instruction, 
	data, 
	trace, 

	count,
};

enum class CpuCacheLevel : u8 {
	l1,
	l2,
	l3,

	count,
};

struct CpuFeatureIndex {
	u8 slot;
	u8 bit;
};

inline CpuFeatureIndex getCpuFeatureIndex(CpuFeature f) {
	CpuFeatureIndex result;
	result.slot = (u32)f >> 5;
	result.bit = (u32)f & 0x1F;
	return result;
}

struct CpuCache {
	u32 count;
	u32 size;
};

struct CpuInfo {

	u32 logicalProcessorCount;
	CpuCache cache[(u32)CpuCacheLevel::count][(u32)CpuCacheType::count];
	u32 cacheLineSize;
	u32 features[ceil((u32)CpuFeature::count, 32u) / 32];
	CpuVendor vendor;
	char brand[49];

	inline bool hasFeature(CpuFeature feature) const {
		CpuFeatureIndex index = getCpuFeatureIndex(feature);
		if (index.slot >= count_of(features))
			return 0;
		return features[index.slot] & (1 << index.bit);
	}
	u32 totalCacheSize(CpuCacheLevel level) const {
		u32 index = (u32)level;
		assert(index < count_of(cache));
		u32 result = 0;
		for (auto &c : cache[index]) {
			result += c.size;
		}
		return result;
	}
#pragma warning(suppress: 4820)
};

TL_API char const *to_string(CpuFeature);
TL_API char const *to_string(CpuVendor);
TL_API CpuInfo get_cpu_info();

#ifdef TL_IMPL
#if OS_WINDOWS

struct CpuidRegisters {
	s32 eax;
	s32 ebx;
	s32 ecx;
	s32 edx;
};

CpuidRegisters cpuid(s32 func) { 
	CpuidRegisters result;
	__cpuid(&result.eax, func);
	return result;
}
CpuidRegisters cpuid(s32 func, s32 subfunc) {
	CpuidRegisters result;
	__cpuidex(&result.eax, func, subfunc); 
	return result;
}

CpuInfo get_cpu_info() {
	CpuInfo result = {};

	DWORD processorInfoLength = 0;
	if (!GetLogicalProcessorInformation(0, &processorInfoLength)) {
		DWORD err = GetLastError();
		assert(err == ERROR_INSUFFICIENT_BUFFER, "GetLastError(): {}", err);
	}

	auto buffer = (SYSTEM_LOGICAL_PROCESSOR_INFORMATION *)malloc(processorInfoLength);
	defer { free(buffer); };

	if (!GetLogicalProcessorInformation(buffer, &processorInfoLength))
		invalid_code_path("GetLogicalProcessorInformation: %u", GetLastError());

	assert(processorInfoLength % sizeof(buffer[0]) == 0);

	
	auto convertCacheType = [](PROCESSOR_CACHE_TYPE v) -> CpuCacheType {
		switch (v) {
			case CacheUnified: return CpuCacheType::unified;
			case CacheInstruction: return CpuCacheType::instruction;
			case CacheData: return CpuCacheType::data;
			case CacheTrace: return CpuCacheType::trace;
		}
		invalid_code_path();
		return {};
	};


	for (auto &info : Span{buffer, processorInfoLength / sizeof(buffer[0])}) {
		switch (info.Relationship) {
			case RelationProcessorCore: result.logicalProcessorCount += count_bits(info.ProcessorMask); break;
			case RelationCache: {
				auto &cache = result.cache[info.Cache.Level - 1][(u8)convertCacheType(info.Cache.Type)];
				cache.size += info.Cache.Size;
				++cache.count;
				result.cacheLineSize = info.Cache.LineSize;
				break;
			}
			default:
				break;
#pragma warning(suppress: 4061)
		}
	}

	s32 ecx1 = 0;
	s32 edx1 = 0;
	s32 ebx7 = 0;
	s32 ecx7 = 0;
	s32 ecx1ex = 0;
	s32 edx1ex = 0;

	StaticList<CpuidRegisters, 64> data;
	StaticList<CpuidRegisters, 64> dataEx;

	s32 highestId = cpuid(0).eax;
	for (s32 i = 0; i <= highestId; ++i) {
		data.push_back(cpuid(i, 0));
	}
	if (data.size() > 0) {
		char vendorName[24];
		((s32 *)vendorName)[0] = data[0].ebx;
		((s32 *)vendorName)[1] = data[0].edx;
		((s32 *)vendorName)[2] = data[0].ecx;
		if (startsWith(vendorName, 24, "GenuineIntel")) {
			result.vendor = CpuVendor::intel;
		} else if (startsWith(vendorName, 24, "AuthenticAMD")) {
			result.vendor = CpuVendor::amd;
		}
	}
	if (data.size() > 1) {
		ecx1 = data[1].ecx;
		edx1 = data[1].edx;
	}
	if (data.size() > 7) {
		ebx7 = data[7].ebx;
		ecx7 = data[7].ecx;
	}

	s32 highestExId = cpuid(0x80000000).eax;
	for (s32 i = 0x80000000; i <= highestExId; ++i) {
		dataEx.push_back(cpuid(i, 0));
	}
	if (dataEx.size() > 1) {
		ecx1ex = dataEx[1].ecx;
		edx1ex = dataEx[1].edx;
	}
	if (dataEx.size() > 4) {
		result.brand[48] = 0;
		memcpy(result.brand + sizeof(CpuidRegisters) * 0, &dataEx[2], sizeof(CpuidRegisters));
		memcpy(result.brand + sizeof(CpuidRegisters) * 1, &dataEx[3], sizeof(CpuidRegisters));
		memcpy(result.brand + sizeof(CpuidRegisters) * 2, &dataEx[4], sizeof(CpuidRegisters));
	} else {
		result.brand[0] = 0;
	}

	auto set = [&](CpuFeature feature, bool value) {
		CpuFeatureIndex index = getCpuFeatureIndex(feature);
		result.features[index.slot] |= (u32)value << index.bit;
	};

	// clang-format off
	set(CpuFeature::sse3,		(ecx1	& (1 <<  0)));
    set(CpuFeature::pclmulqdq,	(ecx1	& (1 <<  1)));
    set(CpuFeature::monitor,	(ecx1	& (1 <<  3)));
    set(CpuFeature::ssse3,		(ecx1	& (1 <<  9)));
    set(CpuFeature::fma,		(ecx1	& (1 << 12)));
    set(CpuFeature::cmpxchg16b,	(ecx1	& (1 << 13)));
    set(CpuFeature::sse41,		(ecx1	& (1 << 19)));
    set(CpuFeature::sse42,		(ecx1	& (1 << 20)));
    set(CpuFeature::movbe,		(ecx1	& (1 << 22)));
    set(CpuFeature::popcnt,		(ecx1	& (1 << 23)));
    set(CpuFeature::aes,		(ecx1	& (1 << 25)));
    set(CpuFeature::xsave,		(ecx1	& (1 << 26)));
    set(CpuFeature::osxsave,	(ecx1	& (1 << 27)));
    set(CpuFeature::avx,		(ecx1	& (1 << 28)));
    set(CpuFeature::f16c,		(ecx1	& (1 << 29)));
    set(CpuFeature::rdrand,		(ecx1	& (1 << 30)));
    set(CpuFeature::msr,		(edx1	& (1 <<  5)));
    set(CpuFeature::cx8,		(edx1	& (1 <<  8)));
    set(CpuFeature::sep,		(edx1	& (1 << 11)));
    set(CpuFeature::cmov,		(edx1	& (1 << 15)));
    set(CpuFeature::clfsh,		(edx1	& (1 << 19)));
    set(CpuFeature::mmx,		(edx1	& (1 << 23)));
    set(CpuFeature::fxsr,		(edx1	& (1 << 24)));
    set(CpuFeature::sse,		(edx1	& (1 << 25)));
    set(CpuFeature::sse2,		(edx1	& (1 << 26)));
    set(CpuFeature::fsgsbase,	(ebx7	& (1 <<  0)));
    set(CpuFeature::bmi1,		(ebx7	& (1 <<  3)));
    set(CpuFeature::hle,		(ebx7	& (1 <<  4)) && result.vendor == CpuVendor::intel);
    set(CpuFeature::avx2,		(ebx7	& (1 <<  5)));
    set(CpuFeature::bmi2,		(ebx7	& (1 <<  8)));
    set(CpuFeature::erms,		(ebx7	& (1 <<  9)));
    set(CpuFeature::invpcid,	(ebx7	& (1 << 10)));
    set(CpuFeature::rtm,		(ebx7	& (1 << 11)) && result.vendor == CpuVendor::intel);
    set(CpuFeature::avx512f,	(ebx7	& (1 << 16)));
    set(CpuFeature::rdseed,		(ebx7	& (1 << 18)));
    set(CpuFeature::adx,		(ebx7	& (1 << 19)));
    set(CpuFeature::avx512pf,	(ebx7	& (1 << 26)));
    set(CpuFeature::avx512er,	(ebx7	& (1 << 27)));
    set(CpuFeature::avx512cd,	(ebx7	& (1 << 28)));
    set(CpuFeature::sha,		(ebx7	& (1 << 29)));
    set(CpuFeature::prefetchwt1,(ecx7	& (1 <<  0)));
    set(CpuFeature::lahf,		(ecx1ex	& (1 <<  0)));
    set(CpuFeature::lzcnt,		(ecx1ex	& (1 <<  5)) && result.vendor == CpuVendor::intel);
    set(CpuFeature::abm,		(ecx1ex	& (1 <<  5)) && result.vendor == CpuVendor::amd);
    set(CpuFeature::sse4a,		(ecx1ex	& (1 <<  6)) && result.vendor == CpuVendor::amd);
    set(CpuFeature::xop,		(ecx1ex	& (1 << 11)) && result.vendor == CpuVendor::amd);
    set(CpuFeature::tbm,		(ecx1ex	& (1 << 21)) && result.vendor == CpuVendor::amd);
    set(CpuFeature::syscall,	(edx1ex	& (1 << 11)) && result.vendor == CpuVendor::intel);
    set(CpuFeature::mmxext,		(edx1ex	& (1 << 22)) && result.vendor == CpuVendor::amd);
    set(CpuFeature::rdtscp,		(edx1ex	& (1 << 27)) && result.vendor == CpuVendor::intel);
    set(CpuFeature::_3dnowext,	(edx1ex	& (1 << 30)) && result.vendor == CpuVendor::amd);
    set(CpuFeature::_3dnow,		(edx1ex	& (1 << 31)) && result.vendor == CpuVendor::amd);
	// clang-format on
	
	return result;
}

#endif // OS_WINDOWS

char const *to_string(CpuFeature f) {
#define CASE(x) case TL::CpuFeature::x: return #x;
	switch (f) {
		CASE(_3dnow)
		CASE(_3dnowext)
		CASE(abm)
		CASE(adx)
		CASE(aes)
		CASE(avx)
		CASE(avx2)
		CASE(avx512cd)
		CASE(avx512er)
		CASE(avx512f)
		CASE(avx512pf)
		CASE(bmi1)
		CASE(bmi2)
		CASE(clfsh)
		CASE(cmov)
		CASE(cmpxchg16b)
		CASE(cx8)
		CASE(erms)
		CASE(f16c)
		CASE(fma)
		CASE(fsgsbase)
		CASE(fxsr)
		CASE(hle)
		CASE(invpcid)
		CASE(lahf)
		CASE(lzcnt)
		CASE(mmx)
		CASE(mmxext)
		CASE(monitor)
		CASE(movbe)
		CASE(msr)
		CASE(osxsave)
		CASE(pclmulqdq)
		CASE(popcnt)
		CASE(prefetchwt1)
		CASE(rdrand)
		CASE(rdseed)
		CASE(rdtscp)
		CASE(rtm)
		CASE(sep)
		CASE(sha)
		CASE(sse)
		CASE(sse2)
		CASE(sse3)
		CASE(sse41)
		CASE(sse42)
		CASE(sse4a)
		CASE(ssse3)
		CASE(syscall)
		CASE(tbm)
		CASE(xop)
		CASE(xsave)
		default: return "unknown";
	}
#undef CASE
}
char const *to_string(CpuVendor v) {
#define CASE(x) case TL::CpuVendor::x: return #x;
	switch (v) {
		CASE(intel)
		CASE(amd)
		default: return "unknown";
	}
#undef CASE
}

#endif // TL_IMPL

}

#pragma warning(pop)
