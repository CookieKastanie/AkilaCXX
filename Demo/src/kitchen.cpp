#include "kitchen.hpp"

using namespace akila;

void Kitchen::loadShaders() {
	Resources::load("bake.json", []() {});
}

void Kitchen::bakeBrdfLut(float texSize) {
	FrameBuffer framebuffer;

	auto tex = Resources::create<Texture2D>("bake_render_target", TextureBuffer::Format::RG);

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

	Texture2DLoader::write("brdf_lut.png", Resources::get<Texture2D>("bake_render_target"));
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


void Kitchen::bakeCubemapFromEqui(Texture2D const *tex, IVec2 size) {
	FrameBuffer framebuffer;

	Ptr<StaticMesh> invertedCube = Ptr<StaticMesh>(StaticMeshPrimitives::invertedCube());

	TextureBuffer::Parameters params;
	params.magFilter = TextureBuffer::FilterMode::LINEAR;
	params.minFilter = TextureBuffer::FilterMode::LINEAR_MIPMAP_LINEAR;
	params.wrapR = TextureBuffer::WrapMode::CLAMP_TO_EDGE;
	params.wrapS = TextureBuffer::WrapMode::CLAMP_TO_EDGE;
	params.wrapT = TextureBuffer::WrapMode::CLAMP_TO_EDGE;

	auto skybox = Resources::create<TextureCubmap>("bake_render_target", TextureBuffer::Format::RGB);
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
