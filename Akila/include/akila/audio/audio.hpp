#pragma once

#include <miniaudio/miniaudio.h>
#include <akila/audio/audio_source.hpp>
#include <akila/audio/audio_emitter.hpp>
#include <string>
#include <deque>

namespace akila {
	class Audio {
	public:
		static void detach(AudioSource *source);
		static void detach(AudioEmitter *source);

	private:
		friend class Core;
		friend class AudioSource;
		friend class AudioEmitter;

		static ma_context context;
		static ma_resource_manager resourceManager;
		static ma_engine engine;
		static ma_device device;

		static std::deque<ma_sound> detachedSounds;

		static void init();
		static void terminate();

		static float lastCheck;
		static void checkDetechedSounds();
	};
}
