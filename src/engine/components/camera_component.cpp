#include "akila/engine/components/camera_component.hpp"

using namespace akila;

unsigned int CameraComponent::nextId = 0;

CameraComponent::CameraComponent(): Camera{}, id{++nextId} {

}

unsigned int CameraComponent::getId() {
	return id;
}

