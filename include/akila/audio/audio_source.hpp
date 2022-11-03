#pragma once

#include "akila/audio/audio.hpp"
#include <string>

namespace akila {
	class AudioSource {
	public:
		AudioSource();
		~AudioSource();

		AudioSource(AudioSource const &other) = delete;
		AudioSource &operator=(AudioSource const &other) = delete;
		
		AudioSource(AudioSource const &&other) noexcept = delete;
		AudioSource &operator=(AudioSource const &&other) noexcept = delete;

		bool decodeFile(std::string const &path);
		bool play();
		bool stop();
		bool isFinished();
		void setVolume(float volume);
		void setLooping(bool loop);

	private:
		friend class Audio;
		friend class AudioEmitter;

		Audio::KeepedSound *sound;
	};
}
