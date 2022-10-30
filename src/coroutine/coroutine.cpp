#include "akila/coroutine/coroutine.hpp"
#include <minicoro/minicoro.h>
#include <iostream>

using namespace akila;

Coroutine::Context::Context(): delay{0}, waiting{false}, mco{nullptr} {

}

Coroutine::Context::~Context() {
	deleteMCO();
}

void Coroutine::Context::deleteMCO() {
	mco_state state = mco_status(mco);
	if(state == MCO_DEAD || state == MCO_SUSPENDED) {
		if(mco != nullptr) mco_destroy(mco);
	}
}

Coroutine::Coroutine(): context{std::unique_ptr<Context>(new Context{})} {}

void Coroutine::retrieveCoroFunc(mco_coro *mco) {
	Context *ctx = (Context *)mco_get_user_data(mco);
	ctx->function(*ctx);
}

void Coroutine::start(std::size_t stackSize, Function const &function) {
	context->deleteMCO();

	mco_desc desc = mco_desc_init(retrieveCoroFunc, stackSize);

	desc.user_data = static_cast<void *>(context.get());

	mco_coro *mco;
	mco_result res = mco_create(&mco, &desc);

	if(res != MCO_SUCCESS) std::cerr << "Impossible de créer la coroutine" << std::endl;

	context->mco = mco;
	context->function = function;

	mco_resume(context->mco);
}

void Coroutine::start(Function const &function) {
	start(0, function);
}

bool Coroutine::isAlive() {
	mco_state state = mco_status(context->mco);
	return state != mco_state::MCO_DEAD;
}

void Coroutine::Context::yield() const {
	mco_yield(mco);
}

void Coroutine::Context::wait(float delta) const {
	delay += delta;
	waiting = true;
	mco_yield(mco);
}

void Coroutine::resume(float delta) {
	if(mco_status(context->mco) != MCO_SUSPENDED) return;

	if(context->waiting) {
		context->delay -= delta;
		if(context->delay <= 0) {
			context->delay = 0;
			context->waiting = false;
		} else {
			return;
		}
	}

	mco_resume(context->mco);
}

void Coroutine::kill() {
	context->deleteMCO();
}
