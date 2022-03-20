#pragma once

#include <Akila/core/Core.hpp>
#include <Akila/graphics/gl/FrameBuffer.hpp>
#include <Akila/entities/Transform.hpp>
#include <vector>

using namespace Akila;

class GabLayer2: public Layer {
private:
	Ref<Shader> gShader;
	Ref<Shader> ssaoShader;
	Ref<Shader> ssaoBlurShader;
	Ref<Shader> deferredShader;
	Ref<Shader> fxaaShader;

	Ref<Mesh> model;
	Ref<Mesh> invertedCube;
	Ref<Mesh> screenTriangle;

	Ptr<FrameBuffer> gFbo;
	Ptr<FrameBuffer> ssaoFbo;
	Ptr<FrameBuffer> ssaoBlurFbo;
	Ptr<FrameBuffer> deferredFbo;

	Ref<Texture> noiseTex;

	///

	bool applySSAO;
	int kernelSize;
	float radius;
	float bias;

	bool useFXAA;

	float textureScale;
	std::function<void()> resizeFunction;

public:
	GabLayer2();

	void draw() override;
	void drawImGui() override;
};
