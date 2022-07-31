#include "akila/audio/audio_buffer.hpp"
#include "akila/audio/audio.hpp"

using namespace akila;

AudioBuffer::AudioBuffer(): source{} {

}

AudioBuffer::~AudioBuffer() {
	ma_resource_manager_data_source_uninit(&source);
}

void AudioBuffer::loadFromFile(std::string const &path) {
	ma_result result = ma_resource_manager_data_source_init(
		ma_engine_get_resource_manager(&Audio::engine),
		path.c_str(),
		MA_RESOURCE_MANAGER_DATA_SOURCE_FLAG_DECODE,
		NULL,
		&source);

	if(result != MA_SUCCESS) {
		std::printf("Failed to load sound \"%s\".", path.c_str());
		std::exit(1);
	}
}
