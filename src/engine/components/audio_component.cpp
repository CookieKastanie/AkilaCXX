#include "akila/engine/components/audio_component.hpp"

using namespace akila;

AudioComponent::AudioComponent() {

}

AudioComponent::AudioComponent(Ref<AudioSource> source): emitter{source} {

}
