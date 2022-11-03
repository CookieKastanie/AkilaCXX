#pragma once

#include "akila/audio/audio_source.hpp"
#include "akila/memory/ref.hpp"
#include "akila/math/math.hpp"

namespace akila {
	class AudioEmitter {
	public:
		AudioEmitter();
		AudioEmitter(Ref<AudioSource> source);
		~AudioEmitter();

		AudioEmitter(AudioEmitter const &other);
		AudioEmitter &operator=(AudioEmitter const &other);

		AudioEmitter(AudioEmitter &&other) noexcept;
		AudioEmitter &operator=(AudioEmitter &&other) noexcept;

		void play();
		void setSource(Ref<AudioSource> source);
		bool isFinished();

		void setPosition(Vec3 const &position);

	private:
		friend class Audio;

		Ref<AudioSource> source;
		Audio::InstancedSound *sound;
	};
}
