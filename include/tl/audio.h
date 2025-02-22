#pragma once
#include "buffer.h"
#include "file.h"

namespace tl {

#pragma pack(push, 1)
struct WavChunk {
	char id[4];
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
	char format[4];
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
	Buffer buffer;
	void *samples = 0;
	u32 sample_count = 0;
	u32 sample_rate = 0;
	u16 num_channels = 0;
	u16 bytes_per_sample = 0;
};

bool validate(WavHeader *header) {
	if (array_as_span(header->chunk.id)       != "RIFF"s) return false;
	if (array_as_span(header->format)         != "WAVE"s) return false;
	if (array_as_span(header->sub_chunk_1.id) != "fmt "s) return false;
	if (header->audio_format     != 1)  return false;
	if (header->sub_chunk_1.size != 16) return false;
	return true;
}

WavChunk *get_samples_chunk(WavHeader *header, u8 *end_of_buffer) {
	StaticList<WavChunk *, 16> sub_chunks;

	for (WavChunk *sub_chunk = &header->sub_chunk_2; (u8 *)sub_chunk < end_of_buffer; sub_chunk = next_chunk(sub_chunk)) {
		sub_chunks.add(sub_chunk);
	}

	WavChunk **it = find_if(sub_chunks, [](WavChunk *subChunk){ return array_as_span(subChunk->id) == "data"s; });

	if (it == sub_chunks.end()) {
		return {};
	}

	return *it;
}

Sound load_wav_from_memory(Span<u8> data) {
	if (data.count < sizeof(WavHeader)) {
		return {};
	}

	auto header = (WavHeader *)data.data;
	if (!validate(header)) {
		return {};
	}
	WavChunk *samples_chunk = get_samples_chunk(header, data.end());

	Sound result = {};
	result.sample_rate      = header->sample_rate;
	result.sample_count     = samples_chunk->size / header->block_align;
	result.num_channels     = header->num_channels;
	result.bytes_per_sample = header->bits_per_sample / 8;

	u32 buffer_size = result.sample_count * result.bytes_per_sample * result.num_channels;
	result.buffer = create_buffer(buffer_size);
	result.samples = result.buffer.data;
	memcpy(result.samples, samples_chunk->data(), buffer_size);

	return result;
}

Sound load_wav_from_file(Span<utf8> path) {
	auto file = read_entire_file(path);
	if (!file.data) return {};

	auto data = file;
	if (data.count < sizeof(WavHeader)) {
		return {};
	}

	auto header = (WavHeader *)data.data;
	if (!validate(header)) {
		return {};
	}
	WavChunk *samples_chunk = get_samples_chunk(header, data.end());

	Sound result = {};
	result.buffer           = file;
	result.sample_rate      = header->sample_rate;
	result.sample_count     = samples_chunk->size / header->block_align;
	result.num_channels     = header->num_channels;
	result.bytes_per_sample = header->bits_per_sample / 8;
	result.samples          = samples_chunk->data();

	return result;
}

void free(Sound &sound) {
	free(sound.buffer);
	sound = {};
}

}
