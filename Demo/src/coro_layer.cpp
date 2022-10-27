#include "coro_layer.hpp"

#include <akila/default/resources/static_mesh_primitives.hpp>
#include <akila/default/systems.hpp>

#include <akila/default/layers.hpp>

using namespace akila;

std::string const shaderCode = R"---(
#akila_vertex

layout(location = a_position_loc) in vec4 a_position;

uniform mat4 model;
uniform mat4 proj;

void main() { 
	gl_Position = proj * model * a_position;
}

#akila_fragment

out vec4 fragColor;

uniform vec3 color;

void main() {	
	fragColor = vec4(color, 1.0);
}
)---";

MainSystem::MainSystem(): System{ECS::createSignature<SquareComponent, TransformComponent>()} {
	shader = Resources::get<Shader>("uni-color");
	quad = Resources::get<StaticMesh>("quad");

}

void MainSystem::update() {
	
}

void MainSystem::draw() {
	Renderer::bindDefaultFrameBuffer();
	Renderer::clear();

	Vec2 winSize = Window::getSize();
	cam.projection = ortho(0.f, winSize.x, 0.f, winSize.y, -2.f, 2.f);

	shader->bind();
	shader->send("proj", cam.projection);

	for(Entity e : entities) {
		auto &sqrtComp = e.getComponent<SquareComponent>();
		auto &transComp = e.getComponent<TransformComponent>();

		shader->send("model", transComp.calcMatrix());
		shader->send("color", sqrtComp.color);

		quad->draw();
	}
}

Entity MainSystem::tryPick(Vec2 pos) {
	for(Entity e : entities) {
		Vec3 epos = e.getComponent<TransformComponent>().position;

		float left = epos.x - 50;
		float right = epos.x + 50;

		float top = epos.y + 50;
		float bottom = epos.y - 50;

		if(pos.x >= left && pos.x <= right && pos.y <= top && pos.y >= bottom) {
			return e;
		}
	}

	return Entity{};
}

///////////////////////


void CoroLayer::onMount() {
	Resources::create<Shader>("uni-color", shaderCode);
	Resources::set<StaticMesh>("quad", StaticMeshPrimitives::quad());

	keyListener = Signals::listen<KeyPressSignal>([this](KeyPressSignal const &e) {
		if(e.key == Inputs::Key::SPACE) {
			spawnQuad();
		}

		if(e.key == Inputs::Key::LEFT_CLICK) {
			grab();
		}
	});

	mainSystem = ECS::createSystem<MainSystem>();
}

void CoroLayer::spawnQuad() {
	Entity e = ECS::createEntity();

	IVec2 winSize = Window::getSize();

	e.addComponent<SquareComponent>({Random::getVec3()});
	e.addComponent<TransformComponent>();
	e.getComponent<TransformComponent>().position = Vec3(Random::getVec2(0.f, min(winSize.x, winSize.y)), Random::getFloat());
	e.getComponent<TransformComponent>().setScale(50);
}

void CoroLayer::grab() {
	Vec2 point = Inputs::getMousePosition();
	point.y = Window::getSize().y - point.y;

	Entity e = mainSystem->tryPick(point);
	if(!e.isValid()) return;

	dragCoro.start([=](Coroutine::Context const &ctx) {
		auto &pos = e.getComponent<TransformComponent>().position;

		float savedZ = pos.z;
		pos.z = 1.5;

		while(Inputs::isPressed(Inputs::Key::LEFT_CLICK)) {
			Vec2 mousePos = Inputs::getMousePosition();
			mousePos.y = Window::getSize().y - mousePos.y;

			pos.x = mousePos.x;
			pos.y = mousePos.y;

			ctx.yield();
		}

		pos.z = savedZ;
	});
}

void CoroLayer::onUnmount() {

}

void CoroLayer::tick() {
	FUNC_TIME_METRIC();
	mainSystem->update();
}

void CoroLayer::frame() {
	dragCoro.resume();
	mainSystem->draw();
}

void CoroLayer::gui() {

}
