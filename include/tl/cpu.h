#pragma once
#include "common.h"
#include "static_list.h"

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

struct CpuCache {
	u32 count;
	u32 size;
};

using CpuFeatureMask = umm;
inline constexpr u32 bits_in_cpu_feature_mask = sizeof(CpuFeatureMask) * 8;

inline CpuFeatureIndex get_cpu_feature_index(CpuFeature f) {
	CpuFeatureIndex result;
	constexpr u32 shift = log2(bits_in_cpu_feature_mask);
	result.slot = (CpuFeatureMask)f >> shift;
	result.bit  = (CpuFeatureMask)f & (bits_in_cpu_feature_mask - 1);
	return result;
}

struct CpuInfo {
	u32 logical_processor_count;
	CpuCache caches_by_level_and_type[CpuCacheLevel_count][CpuCacheType_count];
	u32 cache_line_size;
	CpuFeatureMask feature_masks[ceil<umm>(CpuFeature_count, sizeof(CpuFeatureMask) * 8) / (sizeof(CpuFeatureMask) * 8)];
	CpuVendor vendor;
	char brand[49];

	inline bool has_feature(CpuFeature feature) const {
		CpuFeatureIndex index = get_cpu_feature_index(feature);
		if (index.slot >= count_of(feature_masks))
			return 0;
		return feature_masks[index.slot] & ((umm)1 << index.bit);
	}
	inline u32 total_cache_size(CpuCacheLevel level) const {
		u32 index = (u32)level;
		assert(index < count_of(caches_by_level_and_type));
		u32 result = 0;
		for (auto &c : caches_by_level_and_type[index]) {
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
		assert_equal(err, ERROR_INSUFFICIENT_BUFFER, "GetLastError(): {}", err);
	}

	auto buffer = (SYSTEM_LOGICAL_PROCESSOR_INFORMATION *)malloc(processorInfoLength);
	defer { free(buffer); };

	if (!GetLogicalProcessorInformation(buffer, &processorInfoLength))
		invalid_code_path("GetLogicalProcessorInformation: {}", GetLastError());

	assert_equal(processorInfoLength % sizeof(buffer[0]), 0);


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
			case RelationProcessorCore: result.logical_processor_count += count_bits((u64)info.ProcessorMask); break;
			case RelationCache: {
				auto &cache = result.caches_by_level_and_type[info.Cache.Level - 1][(u8)convertCacheType(info.Cache.Type)];
				cache.size += info.Cache.Size;
				++cache.count;
				result.cache_line_size = info.Cache.LineSize;
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
	if (data.count > 0) {
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
	if (data.count > 1) {
		ecx1 = data[1].ecx;
		edx1 = data[1].edx;
	}
	if (data.count > 7) {
		ebx7 = data[7].ebx;
		ecx7 = data[7].ecx;
	}

	s32 highestExId = cpuid(0x80000000).eax;
	for (s32 i = 0x80000000; i <= highestExId; ++i) {
		data_ex.add(cpuid(i, 0));
	}
	if (data_ex.count > 1) {
		ecx1ex = data_ex[1].ecx;
		edx1ex = data_ex[1].edx;
	}
	if (data_ex.count > 4) {
		result.brand[48] = 0;
		memcpy(result.brand + sizeof(CpuidRegisters) * 0, &data_ex[2], sizeof(CpuidRegisters));
		memcpy(result.brand + sizeof(CpuidRegisters) * 1, &data_ex[3], sizeof(CpuidRegisters));
		memcpy(result.brand + sizeof(CpuidRegisters) * 2, &data_ex[4], sizeof(CpuidRegisters));
	} else {
		result.brand[0] = 0;
	}

	auto set = [&](CpuFeature feature, bool value) {
		CpuFeatureIndex index = get_cpu_feature_index(feature);
		result.feature_masks[index.slot] |= (CpuFeatureMask)value << index.bit;
	};

	set(CpuFeature_sse3,        (ecx1 & (1 << 0)));
	set(CpuFeature_pclmulqdq,   (ecx1 & (1 << 1)));
	set(CpuFeature_monitor,     (ecx1 & (1 << 3)));
	set(CpuFeature_ssse3,       (ecx1 & (1 << 9)));
	set(CpuFeature_fma,         (ecx1 & (1 << 12)));
	set(CpuFeature_cmpxchg16b,  (ecx1 & (1 << 13)));
	set(CpuFeature_sse41,       (ecx1 & (1 << 19)));
	set(CpuFeature_sse42,       (ecx1 & (1 << 20)));
	set(CpuFeature_movbe,       (ecx1 & (1 << 22)));
	set(CpuFeature_popcnt,      (ecx1 & (1 << 23)));
	set(CpuFeature_aes,         (ecx1 & (1 << 25)));
	set(CpuFeature_xsave,       (ecx1 & (1 << 26)));
	set(CpuFeature_osxsave,     (ecx1 & (1 << 27)));
	set(CpuFeature_avx,         (ecx1 & (1 << 28)));
	set(CpuFeature_f16c,        (ecx1 & (1 << 29)));
	set(CpuFeature_rdrand,      (ecx1 & (1 << 30)));
	set(CpuFeature_msr,         (edx1 & (1 << 5)));
	set(CpuFeature_cx8,         (edx1 & (1 << 8)));
	set(CpuFeature_sep,         (edx1 & (1 << 11)));
	set(CpuFeature_cmov,        (edx1 & (1 << 15)));
	set(CpuFeature_clfsh,       (edx1 & (1 << 19)));
	set(CpuFeature_mmx,         (edx1 & (1 << 23)));
	set(CpuFeature_fxsr,        (edx1 & (1 << 24)));
	set(CpuFeature_sse,         (edx1 & (1 << 25)));
	set(CpuFeature_sse2,        (edx1 & (1 << 26)));
	set(CpuFeature_fsgsbase,    (ebx7 & (1 << 0)));
	set(CpuFeature_bmi1,        (ebx7 & (1 << 3)));
	set(CpuFeature_hle,         (ebx7 & (1 << 4)) && result.vendor == CpuVendor_intel);
	set(CpuFeature_avx2,        (ebx7 & (1 << 5)));
	set(CpuFeature_bmi2,        (ebx7 & (1 << 8)));
	set(CpuFeature_erms,        (ebx7 & (1 << 9)));
	set(CpuFeature_invpcid,     (ebx7 & (1 << 10)));
	set(CpuFeature_rtm,         (ebx7 & (1 << 11)) && result.vendor == CpuVendor_intel);
	set(CpuFeature_avx512f,     (ebx7 & (1 << 16)));
	set(CpuFeature_rdseed,      (ebx7 & (1 << 18)));
	set(CpuFeature_adx,         (ebx7 & (1 << 19)));
	set(CpuFeature_avx512pf,    (ebx7 & (1 << 26)));
	set(CpuFeature_avx512er,    (ebx7 & (1 << 27)));
	set(CpuFeature_avx512cd,    (ebx7 & (1 << 28)));
	set(CpuFeature_sha,         (ebx7 & (1 << 29)));
	set(CpuFeature_prefetchwt1, (ecx7 & (1 << 0)));
	set(CpuFeature_lahf,        (ecx1ex & (1 << 0)));
	set(CpuFeature_lzcnt,       (ecx1ex & (1 << 5)) && result.vendor == CpuVendor_intel);
	set(CpuFeature_abm,         (ecx1ex & (1 << 5)) && result.vendor == CpuVendor_amd);
	set(CpuFeature_sse4a,       (ecx1ex & (1 << 6)) && result.vendor == CpuVendor_amd);
	set(CpuFeature_xop,         (ecx1ex & (1 << 11)) && result.vendor == CpuVendor_amd);
	set(CpuFeature_tbm,         (ecx1ex & (1 << 21)) && result.vendor == CpuVendor_amd);
	set(CpuFeature_syscall,     (edx1ex & (1 << 11)) && result.vendor == CpuVendor_intel);
	set(CpuFeature_mmxext,      (edx1ex & (1 << 22)) && result.vendor == CpuVendor_amd);
	set(CpuFeature_rdtscp,      (edx1ex & (1 << 27)) && result.vendor == CpuVendor_intel);
	set(CpuFeature_3dnowext,    (edx1ex & (1 << 30)) && result.vendor == CpuVendor_amd);
	set(CpuFeature_3dnow,       (edx1ex & (1 << 31)) && result.vendor == CpuVendor_amd);

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
