#include "Akila/core/Coroutine.hpp"

using namespace Akila;

CoroutineBase::CoroutineBase(): index{0} {

}

CoroutineBase::~CoroutineBase() {

}

void CoroutineBase::reset() {
	index = 0;
}

void CoroutineBase::stop() {
	index = -1;
}

////////////////////////////////////////////////////

CoroutineManager::CoroutineManager(): threadPool{} {

}

void CoroutineManager::process(QData c) {
	if(c->index < 0 || c->index >= c->infos.size()) return;

	CoroutineBase::Infos &infos = c->infos[c->index];
	switch(infos.trigger) {
		case Trigger::AT_FRAME_START:
			atFrameStart.push(c);
			break;

		case Trigger::AFTER_FIXED_UPDATE:
			afterFixedUpdate.push(c);
			break;

		case Trigger::BEFORE_DRAW:
			beforeDraw.push(c);
			break;
	}
}

void CoroutineManager::start(Ptr<CoroutineBase> coroutine) {
	coroutine->reset();
	process(coroutine);
}

void CoroutineManager::flush(std::queue<QData> &queue) {
	std::size_t count = queue.size();
	while(count--) {
		auto c = std::move(queue.front());
		queue.pop();

		if(c->infos[c->index].executeInThread) {
			threadPool.submit([c]() {
				c->index += c->execute();
			}, [c, this]() {
				process(c);
			});
		} else {
			c->index += c->execute();
			process(c);
		}
	}
}

void CoroutineManager::flushAtFrameStart() {
	flush(atFrameStart);
}

void CoroutineManager::flushAfterFixedUpdate() {
	flush(afterFixedUpdate);
}

void CoroutineManager::flushBeforeDraw() {
	flush(beforeDraw);
}
