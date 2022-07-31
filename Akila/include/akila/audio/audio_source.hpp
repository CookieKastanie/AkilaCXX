#pragma once

#include <miniaudio/miniaudio.h>
#include <string>

namespace akila {
	class AudioSource {
	public:
		AudioSource();
		~AudioSource();

		bool loadFromFile(std::string const &path);
		void play();

	private:
		friend class AudioEmitter;
		std::string path;
		ma_sound sound;
	};
}
