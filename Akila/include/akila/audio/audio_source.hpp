#pragma once

#include <miniaudio/miniaudio.h>
#include <string>

namespace akila {
	class AudioSource {
	public:
		AudioSource();
		~AudioSource();

		bool decodeFile(std::string const &path);
		bool play();

	private:
		friend class Audio;
		friend class AudioEmitter;

		std::string path;
		ma_sound sound;

		bool initSound(ma_sound *otherSound) const;
	};
}
