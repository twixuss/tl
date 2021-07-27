#pragma once
#include "common.h"

#pragma warning(push)
#pragma warning(disable: 4061)

namespace tl {

#define tl_all_cpu_features(f) \
f(3dnow) \
f(3dnowext) \
f(abm) \
f(adx) \
f(aes) \
f(avx) \
f(avx2) \
f(avx512cd) \
f(avx512er) \
f(avx512f) \
f(avx512pf) \
f(bmi1) \
f(bmi2) \
f(clfsh) \
f(cmov) \
f(cmpxchg16b) \
f(cx8) \
f(erms) \
f(f16c) \
f(fma) \
f(fsgsbase) \
f(fxsr) \
f(hle) \
f(invpcid) \
f(lahf) \
f(lzcnt) \
f(mmx) \
f(mmxext) \
f(monitor) \
f(movbe) \
f(msr) \
f(osxsave) \
f(pclmulqdq) \
f(popcnt) \
f(prefetchwt1) \
f(rdrand) \
f(rdseed) \
f(rdtscp) \
f(rtm) \
f(sep) \
f(sha) \
f(sse) \
f(sse2) \
f(sse3) \
f(sse41) \
f(sse42) \
f(sse4a) \
f(ssse3) \
f(syscall) \
f(tbm) \
f(xop) \
f(xsave) \

enum CpuFeature : u8 {
#define f(x) CpuFeature_##x,
	tl_all_cpu_features(f)
#undef f
	CpuFeature_count,
};


#define tl_all_cpu_vendors(f) \
f(unknown) \
f(intel) \
f(amd) \

enum CpuVendor : u8 {
#define f(x) CpuVendor_##x,
	tl_all_cpu_vendors(f)
#undef f
	CpuVendor_count,
};


#define tl_all_cpu_cache_types(f) \
f(unified) \
f(instruction) \
f(data) \
f(trace) \

enum CpuCacheType : u8 {
#define f(x) CpuCacheType_##x,
	tl_all_cpu_cache_types(f)
#undef f
	CpuCacheType_count,
};


#define tl_all_cpu_cache_levels(f) \
f(l1) \
f(l2) \
f(l3) \

enum CpuCacheLevel : u8 {
#define f(x) CpuCacheLevel_##x,
	tl_all_cpu_cache_levels(f)
#undef f
	CpuCacheLevel_count,
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
	CpuCache cache[CpuCacheLevel_count][CpuCacheType_count];
	u32 cacheLineSize;
	u32 features[ceil((u32)CpuFeature_count, 32u) / 32];
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

TL_API CpuInfo get_cpu_info();

TL_API Span<char> to_string(CpuCacheLevel level);
TL_API Span<char> to_string(CpuCacheType type);
TL_API Span<char> to_string(CpuFeature feature);
TL_API Span<char> to_string(CpuVendor vendor);

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
			case CacheUnified: return CpuCacheType_unified;
			case CacheInstruction: return CpuCacheType_instruction;
			case CacheData: return CpuCacheType_data;
			case CacheTrace: return CpuCacheType_trace;
		}
		invalid_code_path();
		return {};
	};


	for (auto &info : Span{buffer, processorInfoLength / sizeof(buffer[0])}) {
		switch (info.Relationship) {
			case RelationProcessorCore: result.logicalProcessorCount += count_bits((ulong_s)info.ProcessorMask); break;
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
	StaticList<CpuidRegisters, 64> data_ex;

	s32 highestId = cpuid(0).eax;
	for (s32 i = 0; i <= highestId; ++i) {
		data.add(cpuid(i, 0));
	}
	if (data.size > 0) {
		char vendorName[24];
		((s32 *)vendorName)[0] = data[0].ebx;
		((s32 *)vendorName)[1] = data[0].edx;
		((s32 *)vendorName)[2] = data[0].ecx;
		if (starts_with(array_as_span(vendorName), "GenuineIntel"s)) {
			result.vendor = CpuVendor_intel;
		} else if (starts_with(array_as_span(vendorName), "AuthenticAMD"s)) {
			result.vendor = CpuVendor_amd;
		}
	}
	if (data.size > 1) {
		ecx1 = data[1].ecx;
		edx1 = data[1].edx;
	}
	if (data.size > 7) {
		ebx7 = data[7].ebx;
		ecx7 = data[7].ecx;
	}

	s32 highestExId = cpuid(0x80000000).eax;
	for (s32 i = 0x80000000; i <= highestExId; ++i) {
		data_ex.add(cpuid(i, 0));
	}
	if (data_ex.size > 1) {
		ecx1ex = data_ex[1].ecx;
		edx1ex = data_ex[1].edx;
	}
	if (data_ex.size > 4) {
		result.brand[48] = 0;
		memcpy(result.brand + sizeof(CpuidRegisters) * 0, &data_ex[2], sizeof(CpuidRegisters));
		memcpy(result.brand + sizeof(CpuidRegisters) * 1, &data_ex[3], sizeof(CpuidRegisters));
		memcpy(result.brand + sizeof(CpuidRegisters) * 2, &data_ex[4], sizeof(CpuidRegisters));
	} else {
		result.brand[0] = 0;
	}

	auto set = [&](CpuFeature feature, bool value) {
		CpuFeatureIndex index = getCpuFeatureIndex(feature);
		result.features[index.slot] |= (u32)value << index.bit;
	};

	set(CpuFeature_sse3,		(ecx1	& (1 <<  0)));
    set(CpuFeature_pclmulqdq,	(ecx1	& (1 <<  1)));
    set(CpuFeature_monitor,	(ecx1	& (1 <<  3)));
    set(CpuFeature_ssse3,		(ecx1	& (1 <<  9)));
    set(CpuFeature_fma,		(ecx1	& (1 << 12)));
    set(CpuFeature_cmpxchg16b,	(ecx1	& (1 << 13)));
    set(CpuFeature_sse41,		(ecx1	& (1 << 19)));
    set(CpuFeature_sse42,		(ecx1	& (1 << 20)));
    set(CpuFeature_movbe,		(ecx1	& (1 << 22)));
    set(CpuFeature_popcnt,		(ecx1	& (1 << 23)));
    set(CpuFeature_aes,		(ecx1	& (1 << 25)));
    set(CpuFeature_xsave,		(ecx1	& (1 << 26)));
    set(CpuFeature_osxsave,	(ecx1	& (1 << 27)));
    set(CpuFeature_avx,		(ecx1	& (1 << 28)));
    set(CpuFeature_f16c,		(ecx1	& (1 << 29)));
    set(CpuFeature_rdrand,		(ecx1	& (1 << 30)));
    set(CpuFeature_msr,		(edx1	& (1 <<  5)));
    set(CpuFeature_cx8,		(edx1	& (1 <<  8)));
    set(CpuFeature_sep,		(edx1	& (1 << 11)));
    set(CpuFeature_cmov,		(edx1	& (1 << 15)));
    set(CpuFeature_clfsh,		(edx1	& (1 << 19)));
    set(CpuFeature_mmx,		(edx1	& (1 << 23)));
    set(CpuFeature_fxsr,		(edx1	& (1 << 24)));
    set(CpuFeature_sse,		(edx1	& (1 << 25)));
    set(CpuFeature_sse2,		(edx1	& (1 << 26)));
    set(CpuFeature_fsgsbase,	(ebx7	& (1 <<  0)));
    set(CpuFeature_bmi1,		(ebx7	& (1 <<  3)));
    set(CpuFeature_hle,		(ebx7	& (1 <<  4)) && result.vendor == CpuVendor_intel);
    set(CpuFeature_avx2,		(ebx7	& (1 <<  5)));
    set(CpuFeature_bmi2,		(ebx7	& (1 <<  8)));
    set(CpuFeature_erms,		(ebx7	& (1 <<  9)));
    set(CpuFeature_invpcid,	(ebx7	& (1 << 10)));
    set(CpuFeature_rtm,		(ebx7	& (1 << 11)) && result.vendor == CpuVendor_intel);
    set(CpuFeature_avx512f,	(ebx7	& (1 << 16)));
    set(CpuFeature_rdseed,		(ebx7	& (1 << 18)));
    set(CpuFeature_adx,		(ebx7	& (1 << 19)));
    set(CpuFeature_avx512pf,	(ebx7	& (1 << 26)));
    set(CpuFeature_avx512er,	(ebx7	& (1 << 27)));
    set(CpuFeature_avx512cd,	(ebx7	& (1 << 28)));
    set(CpuFeature_sha,		(ebx7	& (1 << 29)));
    set(CpuFeature_prefetchwt1,(ecx7	& (1 <<  0)));
    set(CpuFeature_lahf,		(ecx1ex	& (1 <<  0)));
    set(CpuFeature_lzcnt,		(ecx1ex	& (1 <<  5)) && result.vendor == CpuVendor_intel);
    set(CpuFeature_abm,		(ecx1ex	& (1 <<  5)) && result.vendor == CpuVendor_amd);
    set(CpuFeature_sse4a,		(ecx1ex	& (1 <<  6)) && result.vendor == CpuVendor_amd);
    set(CpuFeature_xop,		(ecx1ex	& (1 << 11)) && result.vendor == CpuVendor_amd);
    set(CpuFeature_tbm,		(ecx1ex	& (1 << 21)) && result.vendor == CpuVendor_amd);
    set(CpuFeature_syscall,	(edx1ex	& (1 << 11)) && result.vendor == CpuVendor_intel);
    set(CpuFeature_mmxext,		(edx1ex	& (1 << 22)) && result.vendor == CpuVendor_amd);
    set(CpuFeature_rdtscp,		(edx1ex	& (1 << 27)) && result.vendor == CpuVendor_intel);
    set(CpuFeature_3dnowext,	(edx1ex	& (1 << 30)) && result.vendor == CpuVendor_amd);
    set(CpuFeature_3dnow,		(edx1ex	& (1 << 31)) && result.vendor == CpuVendor_amd);

	return result;
}

#endif // OS_WINDOWS

Span<char> to_string(CpuCacheLevel level) {
	switch (level) {
#define f(x) case CpuCacheLevel_##x: return #x##s;
		tl_all_cpu_cache_levels(f);
#undef f
	}
	return "unknown"s;
}

Span<char> to_string(CpuCacheType type) {
	switch (type) {
#define f(x) case CpuCacheType_##x: return #x##s;
		tl_all_cpu_cache_types(f);
#undef f
	}
	return "unknown"s;
}

Span<char> to_string(CpuFeature feature) {
	switch (feature) {
#define f(x) case CpuFeature_##x: return #x##s;
		tl_all_cpu_features(f);
#undef f
	}
	return "unknown"s;
}

Span<char> to_string(CpuVendor vendor) {
	switch (vendor) {
#define f(x) case CpuVendor_##x: return #x##s;
		tl_all_cpu_vendors(f);
#undef f
	}
	return "unknown"s;
}

#endif // TL_IMPL

}

#pragma warning(pop)
