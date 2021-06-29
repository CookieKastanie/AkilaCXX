#include "Demo/LoadingState.hpp"
#include "Akila/core/Core.hpp"
#include "Akila/core/Time.hpp"
#include <iostream>
#include <string>

#include <chrono>
#include <stdlib.h> 

class TestTask: public Akila::Task {
	public:
		int id;
		int t;
		TestTask(int id) {
			this->id = id;
			t = rand() % 5 + 1;
		}

		void onBackGround() override {
			std::cout << "start background (" << id << ") (" << t << "s) " << std::this_thread::get_id() << std::endl;
			std::this_thread::sleep_for(std::chrono::seconds(t));
			std::cout << "end background (" << id << ") (" << t << "s) " << std::this_thread::get_id() << std::endl;
		}

		void onMain() override {
			std::cout << "wow main (" << id << ") " << std::this_thread::get_id() << std::endl;
		}
};

class TestTask2: public Akila::Task {
public:

	TestTask2() {

	}

	void onBackGround() override {
		std::this_thread::sleep_for(std::chrono::seconds(10));
	}

	void onMain() override {
		std::cout << "EEEEE" << std::endl;
		Akila::Core::taskManager->submit(new TestTask{88});
	}
};

#include "Akila/graphics/ShaderBuilder.hpp"
#include <vector>
#include "glm/vec2.hpp"

LoadingState::LoadingState(): Akila::State{} {
	/*srand(time(NULL));
	Akila::Core::taskManager->submit(new TestTask2{});
	for(int i = 0; i < 10; ++i) {
		Akila::Core::taskManager->submit(new TestTask{i});
		//std::this_thread::sleep_for(std::chrono::microseconds(10));
	}*/

	shader = Akila::ShaderBuilder::buildFromFile("shaders/default.glsl");

	shader->setUBOIndex("akila_camera_ubo", 0);
	shader->setUBOIndex("akila_lights_ubo", 1);

	vertex = std::make_shared<Akila::VBO>(2, Akila::ShaderBuilder::Attributes::A_POSITION);
	vertex->setData(std::vector<glm::vec2>({
		{-1, -1}, {1, -1}, {1, 1},
		{-1, -1}, {1, 1}, {-1, 1}
	}));

	uv = std::make_shared<Akila::VBO>(2, Akila::ShaderBuilder::Attributes::A_UV);
	uv->setData(std::vector<glm::vec2>({
		{0, 0}, {1, 0}, {1, 1},
		{0, 0}, {1, 1}, {0, 1}
	}));

	vao = std::make_shared<Akila::VAO>();
	vao->bind();
	vao->registerVBO(vertex.get());
	vao->registerVBO(uv.get());
	vao->unbind();
}

void LoadingState::update() {
	auto &cam = Akila::Core::renderer->getSharedCamera();

	float t = std::sin(Akila::Time::now);
	glm::vec3 pos{t * t, 0, 0};
	cam->setPosition(pos);
}

void LoadingState::draw() {
	//std::cout << (1.f / Akila::Time::delta) << std::endl;
	//std::cout << Akila::Core::display->getWidth() << std::endl;

	float fps{1.f / Akila::Time::delta};
	Akila::Core::display->setTitle(std::string("FPS ").append(std::to_string(fps)).c_str());
	
	Akila::Core::renderer->useDefaultFrameBuffer();
	shader->bind();
	vao->draw();
}
