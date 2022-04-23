#include "Akila/graphics/pbr/Environment.hpp"

#include "Akila/core/Core.hpp"
#include "Akila/graphics/gl/FrameBuffer.hpp"
#include "Akila/graphics/ShaderBuilder.hpp"
#include "Akila/graphics/MeshPrimitives.hpp"

using namespace Akila;

void Environment::createBRDFLUT(std::string const &name) {
	TextureBuffer::Parameters brdfTexOpts;
	brdfTexOpts.wrapS = TextureBuffer::WrapMode::CLAMP_TO_EDGE;
	brdfTexOpts.wrapT = TextureBuffer::WrapMode::CLAMP_TO_EDGE;
	brdfTexOpts.minFilter = TextureBuffer::FilterMode::LINEAR;
	brdfTexOpts.magFilter = TextureBuffer::FilterMode::LINEAR;

	Core::resourcePool->textures.set(name, new Texture{TextureBuffer::Format::RGB});
	Ref<Texture> brdfLUT = Core::resourcePool->textures.get(name);
	brdfLUT->setSize(512, 512);
	brdfLUT->setParameters(brdfTexOpts);

	FrameBuffer fbo{};

	fbo.setTexture(brdfLUT, 0);
	fbo.prepare();

	auto triangleVAO = Ptr<Mesh>{MeshPrimitives::screenTriangle()};
	auto brdfShader = Ptr<Shader>{ShaderBuilder::build(BRDF_SHADER)};

	fbo.bind();
	brdfShader->bind();
	triangleVAO->draw();
	fbo.unbind();
}

void Environment::createIBL(
	Ref<Texture> input,
	std::string const &skyboxName,
	std::string const &irradianceName,
	std::string const &prefilterName,
	std::function<void()> const &callback
) {
	Core::resourcePool->cubeMaps.set(skyboxName, new CubeMapTexture{TextureBuffer::Format::RGB16F});
	Core::resourcePool->cubeMaps.set(irradianceName, new CubeMapTexture{TextureBuffer::Format::RGB16F});
	Core::resourcePool->cubeMaps.set(prefilterName, new CubeMapTexture{TextureBuffer::Format::RGB16F});

	struct _ {
		Ptr<FrameBuffer> fbo;
		Ptr<Mesh> invertedCube;
		Ref<Texture> input;
		Ref<CubeMapTexture> skybox;
		Ref<CubeMapTexture> irradiance;
		Ref<CubeMapTexture> prefilter;

		glm::mat4 captureProjection;
		glm::mat4 captureViews[6];

		std::function<void()> callback;
	};

	auto coro = Core::coroutines->create(_{
		createPtr<FrameBuffer>(), Ptr<Mesh>{MeshPrimitives::invertedCube()}, input,
		Core::resourcePool->cubeMaps.get(skyboxName),
		Core::resourcePool->cubeMaps.get(irradianceName),
		Core::resourcePool->cubeMaps.get(prefilterName),
		glm::perspective(glm::radians(90.0f), 1.0f, 0.1f, 10.0f),
		{
			glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(1.0f, 0.0f, 0.0f), glm::vec3(0.0f, -1.0f, 0.0f)),
			glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(-1.0f, 0.0f, 0.0f), glm::vec3(0.0f, -1.0f, 0.0f)),
			glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f), glm::vec3(0.0f, 0.0f, 1.0f)),
			glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, -1.0f, 0.0f), glm::vec3(0.0f, 0.0f, -1.0f)),
			glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 1.0f), glm::vec3(0.0f, -1.0f, 0.0f)),
			glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, -1.0f), glm::vec3(0.0f, -1.0f, 0.0f))
		},
		callback
	});

	// skybox
	coro->push(Trigger::BEFORE_DRAW, [](_ &state) {
		TextureBuffer::Parameters cubmapOpts;
		cubmapOpts.magFilter = TextureBuffer::FilterMode::LINEAR;
		cubmapOpts.minFilter = TextureBuffer::FilterMode::LINEAR_MIPMAP_LINEAR;
		cubmapOpts.wrapR = TextureBuffer::WrapMode::CLAMP_TO_EDGE;
		cubmapOpts.wrapS = TextureBuffer::WrapMode::CLAMP_TO_EDGE;
		cubmapOpts.wrapT = TextureBuffer::WrapMode::CLAMP_TO_EDGE;

		state.skybox->setSize(1024, 1024);
		state.skybox->setParameters(cubmapOpts);

		state.fbo->setTexture(state.skybox, 0);
		state.fbo->prepare();

		auto shader = Ptr<Shader>{ShaderBuilder::build(EQUI_TO_CUBE_SHADER)};
		shader->bind();
		state.fbo->bind();

		shader->send("projection", state.captureProjection);
		shader->send("equirectangularMap", 0);
		state.input->bind();

		for(unsigned int i = 0; i < 6; ++i) {
			shader->send("view", state.captureViews[i]);
			state.fbo->changeAttachment(0, GL_TEXTURE_CUBE_MAP_POSITIVE_X + i);

			state.invertedCube->draw();
		}

		state.skybox->generateMipmap();

		state.fbo->unbind();

		return 1;
	});

	// irradiance
	coro->push(Trigger::BEFORE_DRAW, [](_ &state) {
		TextureBuffer::Parameters cubmapOpts;
		cubmapOpts.magFilter = TextureBuffer::FilterMode::LINEAR;
		cubmapOpts.minFilter = TextureBuffer::FilterMode::LINEAR;
		cubmapOpts.wrapR = TextureBuffer::WrapMode::CLAMP_TO_EDGE;
		cubmapOpts.wrapS = TextureBuffer::WrapMode::CLAMP_TO_EDGE;
		cubmapOpts.wrapT = TextureBuffer::WrapMode::CLAMP_TO_EDGE;

		state.irradiance->setSize(32, 32);
		state.irradiance->setParameters(cubmapOpts);
		state.fbo->setTexture(state.irradiance, 0);
		state.fbo->prepare();

		auto irShader = Ptr<Shader>{ShaderBuilder::build(IRRADIANCE_SHADER)};

		irShader->bind();
		state.fbo->bind();
		irShader->send("projection", state.captureProjection);
		irShader->send("environmentMap", 0);
		state.skybox->bind();

		for(unsigned int i = 0; i < 6; ++i) {
			irShader->send("view", state.captureViews[i]);
			state.fbo->changeAttachment(0, GL_TEXTURE_CUBE_MAP_POSITIVE_X + i);

			state.invertedCube->draw();
		}

		state.fbo->unbind();

		return 1;
	});

	// prefilter
	coro->push(Trigger::BEFORE_DRAW, [](_ &state) {
		TextureBuffer::Parameters cubmapOpts;
		cubmapOpts.magFilter = TextureBuffer::FilterMode::LINEAR;
		cubmapOpts.minFilter = TextureBuffer::FilterMode::LINEAR_MIPMAP_LINEAR;
		cubmapOpts.wrapR = TextureBuffer::WrapMode::CLAMP_TO_EDGE;
		cubmapOpts.wrapS = TextureBuffer::WrapMode::CLAMP_TO_EDGE;
		cubmapOpts.wrapT = TextureBuffer::WrapMode::CLAMP_TO_EDGE;

		state.prefilter->setSize(128, 128);
		state.prefilter->setParameters(cubmapOpts);
		state.prefilter->generateMipmap();

		state.fbo->setTexture(state.prefilter, 0);
		state.fbo->prepare();

		auto prefilterShader = Ptr<Shader>{ShaderBuilder::build(PREFILTER_SHADER)};
		prefilterShader->bind();
		prefilterShader->send("projection", state.captureProjection);
		prefilterShader->send("environmentMap", 0);
		prefilterShader->send("resolution", (float)state.skybox->getWidth());
		state.skybox->bind();

		unsigned int maxMipLevels = 5;
		for(unsigned int mip = 0; mip < maxMipLevels; ++mip) {
			// reisze framebuffer according to mip-level size.
			unsigned int mipWidth = state.prefilter->getWidth() * std::pow(0.5, mip);
			unsigned int mipHeight = state.prefilter->getHeight() * std::pow(0.5, mip);
			state.fbo->bindWithSize(mipWidth, mipHeight);

			float roughness = (float)mip / (float)(maxMipLevels - 1);
			prefilterShader->send("roughness", roughness);
			for(unsigned int i = 0; i < 6; ++i) {
				prefilterShader->send("view", state.captureViews[i]);
				state.fbo->changeAttachment(0, GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, mip);

				state.invertedCube->draw();
			}
		}

		state.fbo->unbind();

		return 1;
	});

	coro->push(Trigger::AT_FRAME_START, [](_ &state) {
		state.callback();
		return 1;
	});

	Core::coroutines->start(coro);
}
