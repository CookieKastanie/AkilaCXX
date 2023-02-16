# RHI

## Renderer

```cpp
Renderer::useDefaultFrameBuffer();
Renderer::setClearColor(1.0f, 0.5f, 0.2f, 1.0f);
Renderer::enable(Renderer::Capability::DEPTH_TEST);
Renderer::clear();
```

## Buffers

```cpp
Ptr<VBO> positions = createPtr<VBO>(3, 0); // vec3 sur l'attribut 0
positions->setData(std::vector<Vec2>({
	{3, 1, 0}, {-1, 1, 0}, {-1, -3, 0}
}));

Ptr<VBO> uvs = createPtr<VBO>(2, 1); // vec2 sur l'attribut 1
uv->setData(std::vector<Vec2>({
	{2, 1}, {0, 1}, {0, -1}
}));

Ptr<VAO> vao = createPtr<VAO>();
vao->setDrawMode(VAO::Mode::POINTS); // TRIANGLES par defaut
vao->bind();
vao->registerVBO(positions.get());
vao->registerVBO(uvs.get());
vao->unbind();

//...

vao->draw();
```

## Textures

```cpp
Ref<Texture2D> tex = Ressources::create<Texture2D>(); // Texture2DArray, TextureCubemap, ...

tex->setSize(256, 256);

float data[] = {...};
unsigned int mipLvl = 0;

//            data contient un seul cannal...    de type float
tex->setData(&data, TextureBuffer::Format::RED, TextureBuffer::Type::FLOAT, mipLvl);

// changement des parametres de la texture
TextureBuffer::Parameters params;
params.magFilter = TextureBuffer::FilterMode::LINEAR;
params.minFilter = TextureBuffer::FilterMode::LINEAR;
params.wrapS = TextureBuffer::WrapMode::REPEAT;
params.wrapT = TextureBuffer::WrapMode::REPEAT;
tex->setParameters(params);
```

## Shaders

```cpp
Shader shader{"vert program", "geom program", "frag program"};

// retourne une desciption de tous les uniforms
std::vector<UniformInfos> shader.retreiveUniformInfos();
```

## FrameBuffers

```cpp
FrameBuffer framebuffer{};
framebuffer.setTexture(Resources::create<Texture2D>("colorTarget"), 0); // 0 = index d'attachment
framebuffer.setDepthTexture(
	Resources::create<Texture2D>("depthTarget", TextureBuffer::Format::DEPTH_COMPONENT)
);
framebuffer.resizeAll(Window::getSize());
framebuffer.prepare(); // appeler 1 fois pour commencer à render

//render...
framebuffer.bind(0);

//...

frambuffer.blitToDefault(0);
```
