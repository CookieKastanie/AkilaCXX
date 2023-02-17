#pragma once

#include "akila/core/audio/audio_source.hpp"
#include "akila/core/memory/ref.hpp"
#include "akila/core/math/math.hpp"

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

		void setSource(Ref<AudioSource> source);

		// declaration ici pour avoir du inline
		void play() {
			ma_sound_start(&sound->maSound);
		}

		void stop() {
			ma_sound_stop(&sound->maSound);
		}

		bool isPlaying() {
			return ma_sound_is_playing(&sound->maSound) == MA_TRUE;
		}

		bool isFinished() {
			
			return ma_sound_at_end(&sound->maSound) == MA_TRUE;
		}

		void setVolume(float volume) {
			ma_sound_set_volume(&sound->maSound, volume);
		}

		void setLooping(bool loop) {
			ma_sound_set_looping(&sound->maSound, loop);
		}

		void setPosition(Vec3 const &position) {
			ma_sound_set_position(
				&sound->maSound,
				position.x,
				position.y,
				position.z
			);
		}

		void setVelocity(Vec3 const &velocity) {
			ma_sound_set_velocity(
				&sound->maSound,
				velocity.x,
				velocity.y,
				velocity.z
			);
		}

		void setDirection(Vec3 const &direction) {
			ma_sound_set_direction(
				&sound->maSound,
				direction.x,
				direction.y,
				direction.z
			);
		}

		void setCone(float innerAngle, float outerAngle, float outerGain) {
			ma_sound_set_cone(
				&sound->maSound,
				innerAngle,
				outerAngle,
				outerGain
			);
		}

		void setAttenuationModel(Audio::AttenuationModel model) {
			ma_sound_set_attenuation_model(&sound->maSound, static_cast<ma_attenuation_model>(model));
		}

		void setPitch(float pitch) {
			ma_sound_set_pitch(&sound->maSound, pitch);
		}

	private:
		friend class Audio;

		Ref<AudioSource> source;
		Audio::InstancedSound *sound;
	};
}
