#pragma once

#include <akila/akila.hpp>
#include <akila/default/components.hpp>

struct SquareComponent {
	akila::Vec3 color;
};

class MainSystem: public akila::System {
public:
	MainSystem();

	void update();
	void draw();

	akila::Entity tryPick(akila::Vec2 pos);

private:
	akila::Ref<akila::Shader> shader;
	akila::Ref<akila::StaticMesh> quad;

	akila::CameraComponent cam;
};

class CoroLayer: public akila::Layer {
public:
	void onMount() override;
	void onUnmount() override;

	void tick() override;
	void frame() override;
	void gui() override;

	void spawnQuad();
	void grab();

private:
	akila::Coroutine dragCoro;
	akila::Listener keyListener;
	MainSystem *mainSystem;
};
