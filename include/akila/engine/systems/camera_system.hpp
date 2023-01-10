#pragma once

#include "akila/core/ecs/ecs.hpp"
#include "akila/engine/assets/camera_controler.hpp"
#include "akila/core/memory/ptr.hpp"
#include "akila/core/signals/signals.hpp"
#include <string>
#include <unordered_map>

namespace akila {
	class CameraSystem: public System {
	public:
		CameraSystem();

		void addCameraControler(std::string const &name, Ptr<CameraControler> camera);
		void setCurrentCamera(std::string const &name);
		void setCurrentCamera(Entity e);

		void update();

		CameraData const *getCameraData();

	private:
		bool currentIsEntity;
		Ptr<CameraControler> currentCameraControler;
		Entity currentEntity;

		std::unordered_map<std::string, Ptr<CameraControler>> cameraControlers;

		Listener resizeListener;
	};
}
