#include "Akila/graphics/pbr/Environment.hpp"

#include "Akila/core/Core.hpp"
#include "Akila/graphics/gl/FrameBuffer.hpp"
#include "Akila/graphics/ShaderBuilder.hpp"

#include "Akila/graphics/gl/Error.hpp"

using namespace Akila;

void Environment::createIBL(const std::shared_ptr<Texture> &input,
							std::shared_ptr<CubeMapTexture> &skybox,
							std::shared_ptr<CubeMapTexture> &irradiance,
							std::shared_ptr<CubeMapTexture> &prefilter,
							std::shared_ptr<Mesh> &invertedCube) {

	FrameBuffer fbo{};

	glm::mat4 captureProjection = glm::perspective(glm::radians(90.0f), 1.0f, 0.1f, 10.0f);
	glm::mat4 captureViews[] = {
	   glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(1.0f,  0.0f,  0.0f), glm::vec3(0.0f, -1.0f,  0.0f)),
	   glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(-1.0f,  0.0f,  0.0f), glm::vec3(0.0f, -1.0f,  0.0f)),
	   glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f,  1.0f,  0.0f), glm::vec3(0.0f,  0.0f,  1.0f)),
	   glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, -1.0f,  0.0f), glm::vec3(0.0f,  0.0f, -1.0f)),
	   glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f,  0.0f,  1.0f), glm::vec3(0.0f, -1.0f,  0.0f)),
	   glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f,  0.0f, -1.0f), glm::vec3(0.0f, -1.0f,  0.0f))
	};

	////////////////////////////////////////////////////////////////////////////////
	// Creation de la skybox

	TextureBuffer::Parameters cubmapOpts;
	cubmapOpts.magFilter = TextureBuffer::FilterMode::LINEAR;
	cubmapOpts.minFilter = TextureBuffer::FilterMode::LINEAR_MIPMAP_LINEAR;
	cubmapOpts.wrapR = TextureBuffer::WrapMode::CLAMP_TO_EDGE;
	cubmapOpts.wrapS = TextureBuffer::WrapMode::CLAMP_TO_EDGE;
	cubmapOpts.wrapT = TextureBuffer::WrapMode::CLAMP_TO_EDGE;

	skybox->setSize(1024, 1024);
	skybox->setParameters(cubmapOpts);

	fbo.setTexture(skybox, 0);
	fbo.prepare();

	auto shader = ShaderBuilder::build(EQUI_TO_CUBE_SHADER);
	shader->bind();
	fbo.bind();

	shader->send(shader->getUniformId("projection"), captureProjection);
	shader->send(shader->getUniformId("equirectangularMap"), 0);
	input->bind();

	for(unsigned int i = 0; i < 6; ++i) {
		shader->send(shader->getUniformId("view"), captureViews[i]);
		fbo.changeAttachment(0, GL_TEXTURE_CUBE_MAP_POSITIVE_X + i);

		invertedCube->draw();
	}

	skybox->generateMipmap();

	//fbo.unbind();


	////////////////////////////////////////////////////////////////////////////////
	// Creation de l'irradiance

	cubmapOpts.minFilter = TextureBuffer::FilterMode::LINEAR;
	irradiance->setSize(32, 32);
	irradiance->setParameters(cubmapOpts);
	fbo.setTexture(irradiance, 0);
	fbo.prepare();

	auto irShader = ShaderBuilder::build(IRRADIANCE_SHADER);

	irShader->bind();
	fbo.bind();
	irShader->send(irShader->getUniformId("projection"), captureProjection);
	irShader->send(irShader->getUniformId("environmentMap"), 0);
	skybox->bind();

	for(unsigned int i = 0; i < 6; ++i) {
		irShader->send(irShader->getUniformId("view"), captureViews[i]);
		fbo.changeAttachment(0, GL_TEXTURE_CUBE_MAP_POSITIVE_X + i);

		invertedCube->draw();
	}
	
	//fbo.unbind();

	////////////////////////////////////////////////////////////////////////////////
	// Creation de prefilter

	cubmapOpts.minFilter = TextureBuffer::FilterMode::LINEAR_MIPMAP_LINEAR;
	prefilter->setSize(128, 128);
	prefilter->setParameters(cubmapOpts);
	prefilter->generateMipmap();

	fbo.setTexture(prefilter, 0);
	fbo.prepare();

	auto prefilterShader = ShaderBuilder::build(PREFILTER_SHADER);
	prefilterShader->bind();
	prefilterShader->send(prefilterShader->getUniformId("projection"), captureProjection);
	prefilterShader->send(prefilterShader->getUniformId("environmentMap"), 0);
	prefilterShader->send(prefilterShader->getUniformId("resolution"), (float)skybox->getWidth());
	skybox->bind();
	
	unsigned int maxMipLevels = 5;
	for(unsigned int mip = 0; mip < maxMipLevels; ++mip) {
		// reisze framebuffer according to mip-level size.
		unsigned int mipWidth = prefilter->getWidth() * std::pow(0.5, mip);
		unsigned int mipHeight = prefilter->getHeight() * std::pow(0.5, mip);
		fbo.bindWithSize(mipWidth, mipHeight);

		float roughness = (float)mip / (float)(maxMipLevels - 1);
		prefilterShader->send(prefilterShader->getUniformId("roughness"), roughness);
		for(unsigned int i = 0; i < 6; ++i) {
			prefilterShader->send(prefilterShader->getUniformId("view"), captureViews[i]);
			fbo.changeAttachment(0, GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, mip);

			invertedCube->draw();
		}
	}
	
	fbo.unbind();
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
    //fbo.bind();

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
