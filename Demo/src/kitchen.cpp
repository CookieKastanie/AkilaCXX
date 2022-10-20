#include "kitchen.hpp"

using namespace akila;

void Kitchen::loadShaders() {
	Resources::load("bake.json", []() {});
}

void Kitchen::bakeBrdfLut(std::string const &name, float texSize) {
	FrameBuffer framebuffer;

	auto tex = Resources::create<Texture2D>(name, TextureBuffer::Format::RG);

	TextureBuffer::Parameters params;
	params.wrapS = TextureBuffer::WrapMode::CLAMP_TO_EDGE;
	params.wrapT = TextureBuffer::WrapMode::CLAMP_TO_EDGE;
	params.minFilter = TextureBuffer::FilterMode::LINEAR;
	params.magFilter = TextureBuffer::FilterMode::LINEAR;

	tex->setParameters(params);

	framebuffer.setTexture(tex);
	framebuffer.resizeAll({texSize, texSize});
	framebuffer.prepare();

	auto shader = Resources::get<Shader>("brdf_lut");

	Ptr<StaticMesh> triangle = Ptr<StaticMesh>(StaticMeshPrimitives::screenTriangle());

	framebuffer.bind();
	shader->bind();
	triangle->draw();
	framebuffer.unbind();

	Texture2DLoader::write("brdf_lut.png", Resources::get<Texture2D>(name));
}


#define DEF_PROJ Mat4 captureProjection = perspective(radians(90.0f), 1.0f, 0.1f, 10.0f);
#define DEF_VIEWS Mat4 captureViews[6] = {\
	lookAt(Vec3(0.0f, 0.0f, 0.0f), Vec3( 1.0f,  0.0f,  0.0f), Vec3(0.0f, -1.0f,  0.0f)),\
	lookAt(Vec3(0.0f, 0.0f, 0.0f), Vec3(-1.0f,  0.0f,  0.0f), Vec3(0.0f, -1.0f,  0.0f)),\
	lookAt(Vec3(0.0f, 0.0f, 0.0f), Vec3( 0.0f,  1.0f,  0.0f), Vec3(0.0f,  0.0f,  1.0f)),\
	lookAt(Vec3(0.0f, 0.0f, 0.0f), Vec3( 0.0f, -1.0f,  0.0f), Vec3(0.0f,  0.0f, -1.0f)),\
	lookAt(Vec3(0.0f, 0.0f, 0.0f), Vec3( 0.0f,  0.0f,  1.0f), Vec3(0.0f, -1.0f,  0.0f)),\
	lookAt(Vec3(0.0f, 0.0f, 0.0f), Vec3( 0.0f,  0.0f, -1.0f), Vec3(0.0f, -1.0f,  0.0f))\
};


void Kitchen::bakeCubemapFromEqui(std::string const &name, Texture2D const *tex, IVec2 size) {
	FrameBuffer framebuffer;

	Ptr<StaticMesh> invertedCube = Ptr<StaticMesh>(StaticMeshPrimitives::invertedCube());

	TextureBuffer::Parameters params;
	params.magFilter = TextureBuffer::FilterMode::LINEAR;
	params.minFilter = TextureBuffer::FilterMode::LINEAR_MIPMAP_LINEAR;
	params.wrapR = TextureBuffer::WrapMode::CLAMP_TO_EDGE;
	params.wrapS = TextureBuffer::WrapMode::CLAMP_TO_EDGE;
	params.wrapT = TextureBuffer::WrapMode::CLAMP_TO_EDGE;

	auto skybox = Resources::create<TextureCubmap>(name, TextureBuffer::Format::RGB);
	skybox->setSize(size);
	skybox->setParameters(params);

	DEF_PROJ
	DEF_VIEWS

	framebuffer.setTexture(skybox, 0, FrameBuffer::Attachement::CUBE_MAP_POSITIVE_X);
	framebuffer.prepare();

	auto shader = Resources::get<Shader>("equi_to_cube");
	shader->bind();
	framebuffer.bind();

	shader->send("projection", captureProjection);
	shader->send("equirectangularMap", 0);
	tex->bind();

	FrameBuffer::Attachement attachement = FrameBuffer::Attachement::CUBE_MAP_POSITIVE_X;
	for(unsigned int i = 0; i < 6; ++i) {
		shader->send("view", captureViews[i]);
		framebuffer.changeAttachment(0, attachement);
		attachement = FrameBuffer::nextAttachement(attachement);

		invertedCube->draw();
	}

	skybox->generateMipmap();

	framebuffer.unbind();
}

void Kitchen::bakeIrradiance(std::string const &name, akila::TextureCubmap const *skybox, akila::IVec2 size) {
	FrameBuffer framebuffer;

	Ptr<StaticMesh> invertedCube = Ptr<StaticMesh>(StaticMeshPrimitives::invertedCube());

	TextureBuffer::Parameters params;
	params.magFilter = TextureBuffer::FilterMode::LINEAR;
	params.minFilter = TextureBuffer::FilterMode::LINEAR;
	params.wrapR = TextureBuffer::WrapMode::CLAMP_TO_EDGE;
	params.wrapS = TextureBuffer::WrapMode::CLAMP_TO_EDGE;
	params.wrapT = TextureBuffer::WrapMode::CLAMP_TO_EDGE;

	auto irradiance = Resources::create<TextureCubmap>(name, TextureBuffer::Format::RGB);
	irradiance->setSize(size);
	irradiance->setParameters(params);

	DEF_PROJ
	DEF_VIEWS

	framebuffer.setTexture(irradiance, 0, FrameBuffer::Attachement::CUBE_MAP_POSITIVE_X);
	framebuffer.prepare();

	auto shader = Resources::get<Shader>("irradiance");
	shader->bind();
	framebuffer.bind();

	shader->send("projection", captureProjection);
	shader->send("environmentMap", 0);
	skybox->bind();

	FrameBuffer::Attachement attachement = FrameBuffer::Attachement::CUBE_MAP_POSITIVE_X;
	for(unsigned int i = 0; i < 6; ++i) {
		shader->send("view", captureViews[i]);
		framebuffer.changeAttachment(0, attachement);
		attachement = FrameBuffer::nextAttachement(attachement);

		invertedCube->draw();
	}

	framebuffer.unbind();
}

void Kitchen::bakePrefilter(std::string const &name, akila::TextureCubmap const *skybox, akila::IVec2 size) {
	FrameBuffer framebuffer;

	Ptr<StaticMesh> invertedCube = Ptr<StaticMesh>(StaticMeshPrimitives::invertedCube());

	TextureBuffer::Parameters params;
	params.magFilter = TextureBuffer::FilterMode::LINEAR;
	params.minFilter = TextureBuffer::FilterMode::LINEAR_MIPMAP_LINEAR;
	params.wrapR = TextureBuffer::WrapMode::CLAMP_TO_EDGE;
	params.wrapS = TextureBuffer::WrapMode::CLAMP_TO_EDGE;
	params.wrapT = TextureBuffer::WrapMode::CLAMP_TO_EDGE;

	auto prefilter = Resources::create<TextureCubmap>(name, TextureBuffer::Format::RGB);
	prefilter->setSize(size);
	prefilter->setParameters(params);
	prefilter->generateMipmap();

	DEF_PROJ
	DEF_VIEWS

	framebuffer.setTexture(prefilter, 0, FrameBuffer::Attachement::CUBE_MAP_POSITIVE_X);
	framebuffer.prepare();

	auto shader = Resources::get<Shader>("prefilter");
	shader->bind();
	shader->send("projection", captureProjection);
	shader->send("environmentMap", 0);
	shader->send("resolution", static_cast<float>(skybox->getSize().x));
	skybox->bind();

	unsigned int maxMipLevels = 5;
	Vec2 prefilterSize = prefilter->getSize();
	FrameBuffer::Attachement attachement = FrameBuffer::Attachement::CUBE_MAP_POSITIVE_X;
	for(unsigned int mip = 0; mip < maxMipLevels; ++mip) {
		// reisze framebuffer according to mip-level size.
		float mipPow = std::pow(0.5, mip);
		IVec2 mipSize{prefilterSize.x *mipPow, prefilterSize.y * mipPow};
		framebuffer.bindWithSize(mipSize);

		float roughness = static_cast<float>(mip) / static_cast<float>(maxMipLevels - 1);
		shader->send("roughness", roughness);
		for(unsigned int i = 0; i < 6; ++i) {
			shader->send("view", captureViews[i]);
			framebuffer.changeAttachment(0, attachement, mip);
			attachement = FrameBuffer::nextAttachement(attachement);

			invertedCube->draw();
		}
	}

	framebuffer.unbind();
}
