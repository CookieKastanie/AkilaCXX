#pragma once

#include <miniaudio/miniaudio.h>
// clean windows.h bullshit
#undef near
#undef far

#include "akila/math/math.hpp"
#include <string>
#include <array>

namespace akila {
	class Audio {
	public:
		static void detach(class AudioSource const *source);
		static void detach(class AudioEmitter const *emitter);
		static void detach(class AudioEmitter const &emitter);

		static void setListenerPositionDirection(Vec3 const &pos, Vec3 const &dir);

	private:
		friend class Core;
		friend class AudioSource;
		friend class AudioEmitter;

		static ma_context context;
		static ma_resource_manager resourceManager;
		static ma_engine engine;
		static ma_device device;

		static void init();
		static void terminate();

		struct KeepedSound {
			KeepedSound();

			bool used;
			std::size_t index;
			std::string path;
			ma_sound maSound;
		};
		static std::array<KeepedSound, 512> keepedSounds;

		static KeepedSound *createKeepedSound(std::string const &path);
		static void eraseKeepedSound(KeepedSound *sound);

		struct InstancedSound {
			InstancedSound();

			bool used;
			std::size_t index;
			std::size_t keepdSoundIndex;
			ma_sound maSound;
		};
		static std::array<InstancedSound, 1024> instancedSounds;

		static InstancedSound *createInstancedSound(KeepedSound *keepedSound);
		static void eraseInstancedSound(InstancedSound *sound);

		struct DetachedSound {
			DetachedSound();

			bool used;
			ma_sound maSound;
		};
		static std::array<DetachedSound, 32> detachedSounds;

		static DetachedSound *createDetachedSound(KeepedSound *keepedSound);

		static float lastCheck;
		static void checkDetachedSounds();
	};
}
