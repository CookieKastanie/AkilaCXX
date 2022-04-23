#include "Demo/GabLayer2.hpp"
#include "Demo/MouseCamera.hpp"
#include <imgui/imgui.h>
#include <random>
#include <Akila/graphics/MeshPrimitives.hpp>
#include <Akila/core/Metrics.hpp>

using namespace Akila;

float lerp(float a, float b, float f) {
	return a + f * (b - a);
}

bool sliderFloatDefault(const char *label, float *v, float v_min, float v_max, float v_default, const char *display_format = "%.3f") {
	bool ret = ImGui::SliderFloat(label, v, v_min, v_max, display_format);

	if(ImGui::BeginPopupContextItem(label)) {
		char buf[64];
		std::sprintf(buf, "Reset to %.3f", v_default);

		if(ImGui::MenuItem(buf)) *v = v_default;
		ImGui::MenuItem("Close");
		ImGui::EndPopup();
	}

	return ret;
}

bool sliderIntDefault(const char *label, int *v, int v_min, int v_max, int v_default, const char *display_format = "%.3f") {
	bool ret = ImGui::SliderInt(label, v, v_min, v_max, display_format);

	if(ImGui::BeginPopupContextItem(label)) {
		char buf[64];
		std::sprintf(buf, "Reset to %d", v_default);

		if(ImGui::MenuItem(buf)) *v = v_default;
		ImGui::MenuItem("Close");
		ImGui::EndPopup();
	}

	return ret;
}

GabLayer2::GabLayer2(): applySSAO{true}, kernelSize{64}, radius{.5}, bias{.025}, useFXAA{true}, textureScale{.25} {
	Core::resourcePool->load("gabRes.json");

	auto cam = createPtr<MouseCamera>(Core::display->getMouse());
	cam->setCenter({0, 0.5, 0});
	Core::renderer->setCamera(cam);
	Core::renderer->setClearColor(0, 0, 0);

	gShader = Core::resourcePool->shaders.get("gPass");
	ssaoShader = Core::resourcePool->shaders.get("ssao");
	ssaoBlurShader = Core::resourcePool->shaders.get("ssaoBlur");
	deferredShader = Core::resourcePool->shaders.get("deferred");
	fxaaShader = Core::resourcePool->shaders.get("fxaa");

	model = Core::resourcePool->meshs.get("model");

	Core::resourcePool->meshs.set("screenTriangle", MeshPrimitives::screenTriangle());
	screenTriangle = Core::resourcePool->meshs.get("screenTriangle");

	Core::resourcePool->meshs.set("invertedCube", MeshPrimitives::invertedCube());
	invertedCube = Core::resourcePool->meshs.get("invertedCube");
	
	/////////////////////

	gFbo = createPtr<FrameBuffer>();
	gFbo->setTexture(Core::resourcePool->textures.get("gColor"), 0);
	gFbo->setTexture(Core::resourcePool->textures.get("gPosition"), 1);
	gFbo->setTexture(Core::resourcePool->textures.get("gNormal"), 2);
	gFbo->setDepthTexture(Core::resourcePool->textures.get("gDepth"));

	gFbo->resizeAll(1, 1);
	gFbo->prepare();

	/////////////////////

	// generate ssaoKernel
	// ----------------------
	std::vector<glm::vec3> ssaoKernel;
	std::uniform_real_distribution<float> randomFloats(0.0, 1.0);
	std::default_random_engine generator;
	for(unsigned int i = 0; i < 64; ++i) {
		glm::vec3 sample(randomFloats(generator) * 2.0 - 1.0, randomFloats(generator) * 2.0 - 1.0, randomFloats(generator));
		sample = glm::normalize(sample);
		sample *= randomFloats(generator);
		float scale = float(i) / 64.0f;

		// scale samples s.t. they're more aligned to center of kernel
		scale = lerp(0.1f, 1.0f, scale * scale);
		sample *= scale;
		ssaoKernel.push_back(sample);
	}

	// generate noise texture
	// ----------------------
	std::vector<glm::vec3> ssaoNoise;
	for(unsigned int i = 0; i < 16; i++) {
		glm::vec3 noise(randomFloats(generator) * 2.0 - 1.0, randomFloats(generator) * 2.0 - 1.0, 0.0f); // rotate around z-axis (in tangent space)
		ssaoNoise.push_back(noise);
	}
	
	noiseTex = Core::resourcePool->textures.get("noiseTex");
	noiseTex->setSize(4, 4);
	noiseTex->setData(ssaoNoise.data(), Texture::Format::RGB, Texture::Type::FLOAT);
	

	ssaoShader->bind();
	ssaoShader->send("samples[0]", ssaoKernel);
	ssaoShader->send("gPosition", 0);
	ssaoShader->send("gNormal", 1);
	ssaoShader->send("texNoise", 2);


	ssaoFbo = createPtr<FrameBuffer>();
	ssaoFbo->setTexture(Core::resourcePool->textures.get("ssaoRenderTarget"));
	ssaoFbo->resizeAll(1, 1);
	ssaoFbo->prepare();

	ssaoBlurFbo = createPtr<FrameBuffer>();
	ssaoBlurFbo->setTexture(Core::resourcePool->textures.get("ssaoBlurRenderTarget"));
	ssaoBlurFbo->resizeAll(1, 1);
	ssaoBlurFbo->prepare();

	ssaoBlurShader->bind();
	ssaoBlurShader->send("ssaoInput", 0);


	deferredShader->bind();
	deferredShader->send("gAlbedo", 0);
	deferredShader->send("gPosition", 1);
	deferredShader->send("gNormal", 2);
	deferredShader->send("ssao", 3);


	deferredFbo = createPtr<FrameBuffer>();
	deferredFbo->setTexture(Core::resourcePool->textures.get("deferedRenderTarget"), 0);
	deferredFbo->resizeAll(1, 1);
	deferredFbo->prepare();

	fxaaShader->bind();
	fxaaShader->send("texin", 0);

	resizeFunction = [this]() {
		gFbo->resizeAll(Core::display->getWidth(), Core::display->getHeight());
		deferredFbo->resizeAll(Core::display->getWidth(), Core::display->getHeight());

		ssaoFbo->resizeAll(Core::display->getWidth() * textureScale, Core::display->getHeight() * textureScale);
		ssaoBlurFbo->resizeAll(Core::display->getWidth() * textureScale, Core::display->getHeight() * textureScale);
	};

	Core::display->onResize(resizeFunction);
	resizeFunction();
};

void GabLayer2::draw() {
	FUNC_TIME_METRIC();

	gFbo->bind();
	gShader->bind();
	Core::renderer->clear();

	Transform modelTransform;
	gShader->send("model", modelTransform.toMatrix());
	gShader->send("color", glm::vec3(0.8, 0.6, 0.0));
	model->draw();

	modelTransform.setScale(5);
	gShader->send("model", modelTransform.toMatrix());
	gShader->send("color", glm::vec3(0.0, 0.6, 0.8));
	invertedCube->draw();
	
	///

	Core::renderer->disable(Renderer::Capability::DEPTH_TEST);
	Core::renderer->disable(Renderer::Capability::BLEND);
	Core::renderer->disable(Renderer::Capability::CULL_FACE);

	ssaoFbo->bind();
	ssaoShader->bind();
	ssaoShader->send("kernelSize", kernelSize);
	ssaoShader->send("radius", radius);
	ssaoShader->send("bias", bias);

	gFbo->getTexture(1)->bind(0);
	gFbo->getTexture(2)->bind(1);
	noiseTex->bind(2);
	screenTriangle->draw();

	///

	ssaoBlurFbo->bind();
	ssaoBlurShader->bind();
	ssaoFbo->getTexture(0)->bind(0);
	screenTriangle->draw();

	///

	deferredFbo->bind();
	deferredShader->bind();
	deferredShader->send("applySSAO", applySSAO);

	gFbo->getTexture(0)->bind(0);
	gFbo->getTexture(1)->bind(1);
	gFbo->getTexture(2)->bind(2);
	ssaoBlurFbo->getTexture(0)->bind(3);
	screenTriangle->draw();

	///

	Core::renderer->useDefaultFrameBuffer();

	if(useFXAA) {
		fxaaShader->bind();
		deferredFbo->getTexture(0)->bind(0);
		screenTriangle->draw();
	} else {
		deferredFbo->blitToDisplay(0, Core::display.get());
	}

	Core::renderer->enable(Renderer::Capability::CULL_FACE);
	Core::renderer->enable(Renderer::Capability::BLEND);
	Core::renderer->enable(Renderer::Capability::DEPTH_TEST);
}

void GabLayer2::drawImGui() {
	int w = 300;
	int h = ((float)Core::display->getHeight() / (float)Core::display->getWidth()) * w;
	
	ImGui::Begin("Buffers");
	ImGui::Image((ImTextureID)gFbo->getTexture(0)->getId(), ImVec2(w, h), ImVec2(0, 1), ImVec2(1, 0));
	ImGui::Image((ImTextureID)gFbo->getTexture(1)->getId(), ImVec2(w, h), ImVec2(0, 1), ImVec2(1, 0));
	ImGui::Image((ImTextureID)gFbo->getTexture(2)->getId(), ImVec2(w, h), ImVec2(0, 1), ImVec2(1, 0));
	ImGui::End();

	ImGui::Begin("SSAO");
	ImGui::Image((ImTextureID)ssaoFbo->getTexture(0)->getId(), ImVec2(w, h), ImVec2(0, 1), ImVec2(1, 0));
	ImGui::Image((ImTextureID)ssaoBlurFbo->getTexture(0)->getId(), ImVec2(w, h), ImVec2(0, 1), ImVec2(1, 0));
	//ImGui::Image((ImTextureID)deferredFbo->getTexture(0)->getId(), ImVec2(w, h), ImVec2(0, 1), ImVec2(1, 0));
	ImGui::End();

	ImGui::Begin("Options");
	ImGui::Checkbox("SSAO", &applySSAO);
	ImGui::SameLine();
	ImGui::Checkbox("FXAA", &useFXAA);
	sliderIntDefault("Kernel size", &kernelSize, 0, 64, 64);
	sliderFloatDefault("Radius", &radius, 0, 4, .5);
	sliderFloatDefault("Bias", &bias, 0, 1, .025);

	const char *items[] = {"1", "1/2", "1/4", "1/8"};
	const float values[] = {1, 1./2., 1./4., 1./8.};
	static const char *current_item = items[2];
	if(ImGui::BeginCombo("##combo", current_item)) {
		for(int n = 0; n < IM_ARRAYSIZE(items); n++) {
			bool is_selected = (current_item == items[n]);
			if(ImGui::Selectable(items[n], is_selected)) {
				current_item = items[n];
				textureScale = values[&items[n] - &items[0]];
			}

			if(is_selected) ImGui::SetItemDefaultFocus();
		}

		ImGui::EndCombo();
	}

	ImGui::SameLine();
	if(ImGui::Button("Apply")) resizeFunction();

	ImGui::End();
}