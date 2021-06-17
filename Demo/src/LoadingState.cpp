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

LoadingState::LoadingState(): Akila::State{} {
	srand(time(NULL));
	Akila::Core::taskManager->submit(new TestTask2{});
	for(int i = 0; i < 10; ++i) {
		Akila::Core::taskManager->submit(new TestTask{i});
		//std::this_thread::sleep_for(std::chrono::microseconds(10));
	}
}

void LoadingState::update() {

}

void LoadingState::draw() {
	//std::cout << (1.f / Akila::Time::delta) << std::endl;
	//std::cout << Akila::Core::display->getWidth() << std::endl;

	float fps{1.f / Akila::Time::delta};
	Akila::Core::display->setTitle(std::string("FPS ").append(std::to_string(fps)).c_str());
}
