#pragma once

#include <miniaudio/miniaudio.h>
#include <string>

namespace akila {
	class AudioSource {
	public:
		AudioSource();
		~AudioSource();

		bool loadFromFile(std::string const &path);
		bool play();

	private:
		friend class AudioEmitter;
		ma_sound sound;
	};
}
