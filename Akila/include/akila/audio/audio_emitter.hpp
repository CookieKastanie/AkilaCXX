#pragma once

#include <miniaudio/miniaudio.h>
#include "akila/audio/audio_buffer.hpp"

namespace akila {
	class AudioEmitter {
	public:
		AudioEmitter() = default;
		AudioEmitter(AudioBuffer &buffer);
		AudioEmitter(AudioEmitter &audioEmmiter);
		~AudioEmitter();

		void play();

	private:
		ma_sound sound;
	};
}
