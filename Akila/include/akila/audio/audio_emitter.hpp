#pragma once

#include <miniaudio/miniaudio.h>
#include "akila/audio/audio_source.hpp"

namespace akila {
	class AudioEmitter {
	public:
		AudioEmitter() = default;
		AudioEmitter(AudioSource &buffer);
		AudioEmitter(AudioEmitter &audioEmmiter);
		~AudioEmitter();

		void play();
		bool isFinished();

	private:
		friend class Audio;

		ma_sound sound;
	};
}
