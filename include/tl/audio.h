#pragma once
#include "buffer.h"

namespace TL {

#pragma pack(push, 1)
struct WavChunk {
	u32 id;
	u32 size;
	void *data() {
		return this + 1;
	}
};
WavChunk *next_chunk(WavChunk *chunk) {
	return (WavChunk *)((u8 *)chunk->data() + chunk->size);
}

struct WavHeader {
	WavChunk chunk;
	u32 format;
	WavChunk sub_chunk_1;
	u16 audio_format;
	u16 num_channels;
	u32 sample_rate;
	u32 byte_rate;
	u16 block_align;
	u16 bits_per_sample;
	WavChunk sub_chunk_2;
};
#pragma pack(pop)

struct Sound {
	Allocator allocator = current_allocator;
	void *samples = 0;
	u32 sample_count = 0;
	u32 sample_rate = 0;
	u16 num_channels = 0;
	u16 bytes_per_sample = 0;
};

Sound load_wav_from_memory(Span<u8> data) {
	if (data.size < sizeof(WavHeader)) {
		return {};
	}

	auto &header = *(WavHeader *)data.data;

	if (header.chunk.id != *(u32 *)"RIFF")
		return {};
	if (header.format != *(u32 *)"WAVE")
		return {};
	if (header.sub_chunk_1.id != *(u32 *)"fmt ")
		return {};
	if (header.audio_format != 1)
		return {};
	if (header.sub_chunk_1.size != 16)
		return {};

	StaticList<WavChunk *, 16> sub_chunks;

	for (WavChunk *sub_chunk = &header.sub_chunk_2; (u8 *)sub_chunk < data.end(); sub_chunk = next_chunk(sub_chunk)) {
		sub_chunks.add(sub_chunk);
	}

	WavChunk **it = find_if(sub_chunks, [](WavChunk *subChunk){ return subChunk->id == *(u32 *)"data"; });

	if (it == sub_chunks.end()) {
		return {};
	}

	WavChunk *samples_chunk = *it;

	Sound result = {};
	result.sample_rate = header.sample_rate;
	result.sample_count = samples_chunk->size / header.block_align;
	result.num_channels = header.num_channels;
	result.bytes_per_sample = header.bits_per_sample / 8;

	u32 buffer_size = result.sample_count * result.bytes_per_sample * result.num_channels;
	result.samples = ALLOCATE(u8, result.allocator, buffer_size);
	memcpy(result.samples, samples_chunk->data(), buffer_size);

	return result;
}

Sound load_wav_from_file(Span<char> path) {
	auto file = with(temporary_allocator, read_entire_file(path));
	if (!file.data) return {};

	return load_wav_from_memory(file);
}

void free(Sound &sound) {
	FREE(sound.allocator, sound.samples);
	sound = {};
}

}
