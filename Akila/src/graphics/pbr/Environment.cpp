#include "Akila/graphics/pbr/Environment.hpp"

#include "Akila/core/Core.hpp"
#include "Akila/graphics/gl/FrameBuffer.hpp"
#include "Akila/graphics/ShaderBuilder.hpp"

using namespace Akila;

struct SharedData {
	std::shared_ptr<FrameBuffer> fbo;
	glm::mat4 captureProjection;
	glm::mat4 captureViews[6];

	std::shared_ptr<Texture> input;
	std::shared_ptr<CubeMapTexture> skybox;
	std::shared_ptr<CubeMapTexture> irradiance;
	std::shared_ptr<CubeMapTexture> prefilter;
	std::shared_ptr<Mesh> invertedCube;
};

class SkyboxTask: public Task {
private:
	std::shared_ptr<SharedData> data;

public:
	SkyboxTask(const std::shared_ptr<SharedData> &data): data{data} {

	}

	void onMain() override {
		TextureBuffer::Parameters cubmapOpts;
		cubmapOpts.magFilter = TextureBuffer::FilterMode::LINEAR;
		cubmapOpts.minFilter = TextureBuffer::FilterMode::LINEAR_MIPMAP_LINEAR;
		cubmapOpts.wrapR = TextureBuffer::WrapMode::CLAMP_TO_EDGE;
		cubmapOpts.wrapS = TextureBuffer::WrapMode::CLAMP_TO_EDGE;
		cubmapOpts.wrapT = TextureBuffer::WrapMode::CLAMP_TO_EDGE;

		if(data->skybox->getWidth() <= 0 || data->skybox->getHeight() <= 0) {
			data->skybox->setSize(1024, 1024);
		}
		data->skybox->setParameters(cubmapOpts);

		data->fbo->setTexture(data->skybox, 0);
		data->fbo->prepare();

		auto shader = ShaderBuilder::build(EQUI_TO_CUBE_SHADER);
		shader->bind();
		data->fbo->bind();

		shader->send(shader->getUniformId("projection"), data->captureProjection);
		shader->send(shader->getUniformId("equirectangularMap"), 0);
		data->input->bind();

		for(unsigned int i = 0; i < 6; ++i) {
			shader->send(shader->getUniformId("view"), data->captureViews[i]);
			data->fbo->changeAttachment(0, GL_TEXTURE_CUBE_MAP_POSITIVE_X + i);

			data->invertedCube->draw();
		}

		data->skybox->generateMipmap();

		data->fbo->unbind();
	}
};

class IrradianceTask: public Task {
private:
	std::shared_ptr<SharedData> data;

public:
	IrradianceTask(const std::shared_ptr<SharedData> &data): data{data} {

	}

	void onMain() override {
		TextureBuffer::Parameters cubmapOpts;
		cubmapOpts.magFilter = TextureBuffer::FilterMode::LINEAR;
		cubmapOpts.minFilter = TextureBuffer::FilterMode::LINEAR;
		cubmapOpts.wrapR = TextureBuffer::WrapMode::CLAMP_TO_EDGE;
		cubmapOpts.wrapS = TextureBuffer::WrapMode::CLAMP_TO_EDGE;
		cubmapOpts.wrapT = TextureBuffer::WrapMode::CLAMP_TO_EDGE;

		if(data->irradiance->getWidth() <= 0 || data->irradiance->getHeight() <= 0) {
			data->irradiance->setSize(32, 32);
		}
		data->irradiance->setParameters(cubmapOpts);
		data->fbo->setTexture(data->irradiance, 0);
		data->fbo->prepare();

		auto irShader = ShaderBuilder::build(IRRADIANCE_SHADER);

		irShader->bind();
		data->fbo->bind();
		irShader->send(irShader->getUniformId("projection"), data->captureProjection);
		irShader->send(irShader->getUniformId("environmentMap"), 0);
		data->skybox->bind();

		for(unsigned int i = 0; i < 6; ++i) {
			irShader->send(irShader->getUniformId("view"), data->captureViews[i]);
			data->fbo->changeAttachment(0, GL_TEXTURE_CUBE_MAP_POSITIVE_X + i);

			data->invertedCube->draw();
		}

		data->fbo->unbind();
	}
};

class PrefilterTask: public Task {
private:
	std::shared_ptr<SharedData> data;

public:
	PrefilterTask(const std::shared_ptr<SharedData> &data): data{data} {

	}

	void onMain() override {
		TextureBuffer::Parameters cubmapOpts;
		cubmapOpts.magFilter = TextureBuffer::FilterMode::LINEAR;
		cubmapOpts.minFilter = TextureBuffer::FilterMode::LINEAR_MIPMAP_LINEAR;
		cubmapOpts.wrapR = TextureBuffer::WrapMode::CLAMP_TO_EDGE;
		cubmapOpts.wrapS = TextureBuffer::WrapMode::CLAMP_TO_EDGE;
		cubmapOpts.wrapT = TextureBuffer::WrapMode::CLAMP_TO_EDGE;

		if(data->prefilter->getWidth() <= 0 || data->prefilter->getHeight() <= 0) {
			data->prefilter->setSize(128, 128);
		}
		data->prefilter->setParameters(cubmapOpts);
		data->prefilter->generateMipmap();

		data->fbo->setTexture(data->prefilter, 0);
		data->fbo->prepare();

		auto prefilterShader = ShaderBuilder::build(PREFILTER_SHADER);
		prefilterShader->bind();
		prefilterShader->send(prefilterShader->getUniformId("projection"), data->captureProjection);
		prefilterShader->send(prefilterShader->getUniformId("environmentMap"), 0);
		prefilterShader->send(prefilterShader->getUniformId("resolution"), (float)data->skybox->getWidth());
		data->skybox->bind();

		unsigned int maxMipLevels = 5;
		for(unsigned int mip = 0; mip < maxMipLevels; ++mip) {
			// reisze framebuffer according to mip-level size.
			unsigned int mipWidth = data->prefilter->getWidth() * std::pow(0.5, mip);
			unsigned int mipHeight = data->prefilter->getHeight() * std::pow(0.5, mip);
			data->fbo->bindWithSize(mipWidth, mipHeight);

			float roughness = (float)mip / (float)(maxMipLevels - 1);
			prefilterShader->send(prefilterShader->getUniformId("roughness"), roughness);
			for(unsigned int i = 0; i < 6; ++i) {
				prefilterShader->send(prefilterShader->getUniformId("view"), data->captureViews[i]);
				data->fbo->changeAttachment(0, GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, mip);

				data->invertedCube->draw();
			}
		}

		data->fbo->unbind();
	}
};

void Environment::createIBL(const std::shared_ptr<Texture> &input,
							std::shared_ptr<CubeMapTexture> &skybox,
							std::shared_ptr<CubeMapTexture> &irradiance,
							std::shared_ptr<CubeMapTexture> &prefilter,
							std::shared_ptr<Mesh> &invertedCube) {

	auto data = std::make_shared<SharedData>();

	data->fbo = std::make_shared<FrameBuffer>();

	data->captureProjection = glm::perspective(glm::radians(90.0f), 1.0f, 0.1f, 10.0f);

	data->captureViews[0] = glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(1.0f, 0.0f, 0.0f), glm::vec3(0.0f, -1.0f, 0.0f));
	data->captureViews[1] = glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(-1.0f, 0.0f, 0.0f), glm::vec3(0.0f, -1.0f, 0.0f));
	data->captureViews[2] = glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f), glm::vec3(0.0f, 0.0f, 1.0f));
	data->captureViews[3] = glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, -1.0f, 0.0f), glm::vec3(0.0f, 0.0f, -1.0f));
	data->captureViews[4] = glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 1.0f), glm::vec3(0.0f, -1.0f, 0.0f));
	data->captureViews[5] = glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, -1.0f), glm::vec3(0.0f, -1.0f, 0.0f));

	data->input = input;
	data->skybox = skybox;
	data->irradiance = irradiance;
	data->prefilter = prefilter;
	data->invertedCube = invertedCube;


	Core::taskManager->submit(new SkyboxTask{data}); 
	Core::taskManager->submit(new IrradianceTask{data});
	Core::taskManager->submit(new PrefilterTask{data});
}

std::shared_ptr<Texture> Environment::createBRDFLUT() {
	////////////////////////////////////////////////////////////////////////////////
	// Creation de brdfLUT

	TextureBuffer::Parameters brdfTexOpts;
	brdfTexOpts.wrapS = TextureBuffer::WrapMode::CLAMP_TO_EDGE;
	brdfTexOpts.wrapT = TextureBuffer::WrapMode::CLAMP_TO_EDGE;
	brdfTexOpts.minFilter = TextureBuffer::FilterMode::LINEAR;
    brdfTexOpts.magFilter = TextureBuffer::FilterMode::LINEAR;

	auto brdfLUT = std::make_shared<Texture>(TextureBuffer::Format::RGB);
	brdfLUT->setSize(512, 512);
    brdfLUT->setParameters(brdfTexOpts);

    FrameBuffer fbo{};

	fbo.setTexture(brdfLUT, 0);
	fbo.prepare();

    
	auto triangleVAO = Core::resourcesBucket->getMesh("akila_triangle");

	auto brdfShader = ShaderBuilder::build(BRDF_SHADER);

	fbo.bind();
	brdfShader->bind();
	triangleVAO->draw();
	fbo.unbind();

	return brdfLUT;
}
