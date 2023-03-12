#pragma once

#include "akila/core/audio/audio_emitter.hpp"

namespace akila {
	class AudioComponent {
	public:
		AudioComponent();
		AudioComponent(Ref<AudioSource> source);

		void setSource(Ref<AudioSource> source) {
			emitter.setSource(source);
		}

		void play() {
			emitter.play();
		}

		void stop() {
			emitter.stop();
		}

		bool isPlaying() {
			return emitter.isPlaying();
		}

		bool isFinished() {
			return emitter.isFinished();
		}

		void setVolume(float volume) {
			emitter.setVolume(volume);
		}

		void setLooping(bool loop) {
			emitter.setLooping(loop);
		}

		void setCone(float innerAngle, float outerAngle, float outerGain) {
			emitter.setCone(innerAngle, outerAngle, outerGain);
		}

		void setAttenuationModel(Audio::AttenuationModel model) {
			emitter.setAttenuationModel(model);
		}

		void setPitch(float pitch) {
			emitter.setPitch(pitch);
		}

	private:
		friend class AudioSystem;

		AudioEmitter emitter;
		Vec3 lastWorldPosition;
	};
}
