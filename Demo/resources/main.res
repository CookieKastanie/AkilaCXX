shader {
	name: loadingAnimation
	src: shaders/loadingAnimation.glsl

	uniform: color = [1.5, 2.9, 3.5]
}

shader {
	name: textureTest
	src: shaders/textureTest.glsl

	uniform: tex = i[0]
}

shader {
	name: pbr
	src: shaders/pbr.glsl

	uniform: exposure = [1]
	uniform: gamma = [2.2]

	uniform: albedoSampler = i[0]
	uniform: normalSampler = i[1]
	uniform: metallicSampler = i[2]
	uniform: roughnessSampler = i[3]
	uniform: aoSampler = i[4]

	uniform: irradianceMap = i[5]
	uniform: prefilterMap = i[6]
	uniform: brdfLUT = i[7]
}

shader {
	name: skybox
	src: shaders/skybox.glsl

	uniform: exposure = [1]
	uniform: gamma = [2.2]

	uniform: cubemapTexture = i[0]
}

////////////////////////////////////////////////////////////////////////////////////

texture {
	name: citron
	src: textures/citron.png
	format: RGBA
}

texture {
	name: env
	src: textures/env.jpg
	format: RGB
}

texture {
	name: font
	src: fonts/FredokaOne-Regular.png
	format: RGBA
}

texture {
	name: sword_A
	src: textures/sword/sword_A.png
	format: SRGB
	mips: true
	minFilter: LINEAR_MIPMAP_LINEAR
	magFilter: LINEAR_MIPMAP_LINEAR
}

texture {
	name: sword_N
	src: textures/sword/sword_N.png
	format: RGB
	mips: true
	minFilter: LINEAR_MIPMAP_LINEAR
	magFilter: LINEAR_MIPMAP_LINEAR
}

texture {
	name: sword_M
	src: textures/sword/sword_M.png
	format: RED
	mips: true
	minFilter: LINEAR_MIPMAP_LINEAR
	magFilter: LINEAR_MIPMAP_LINEAR
}

texture {
	name: sword_R
	src: textures/sword/sword_R.png
	format: RED
	mips: true
	minFilter: LINEAR_MIPMAP_LINEAR
	magFilter: LINEAR_MIPMAP_LINEAR
}

texture {
	name: sword_AO
	src: textures/sword/sword_AO.png
	format: RED
	mips: true
	minFilter: LINEAR_MIPMAP_LINEAR
	magFilter: LINEAR_MIPMAP_LINEAR
}

////////////////////////////////////////////////////////////////////////////////////

mesh {
	name: sword
	src: meshs/sword.ob
}

mesh {
	name: invertedCube
	src: meshs/invertedCube.ob
}

////////////////////////////////////////////////////////////////////////////////////

material {
	name: loadingScreen
	shader: loadingAnimation
}

material {
	name: textureTest
	shader: textureTest

	texture: sword_A = 0
}

material {
	name: sword
	shader: pbr

	texture: sword_A = 0
	texture: sword_N = 1
	texture: sword_M = 2
	texture: sword_R = 3
	texture: sword_AO = 4

	cubemap: irradiance = 5
	cubemap: prefilter = 6
	texture: brdfLUT = 7
}

material {
	name: brdfLUT
	shader: textureTest

	texture: brdfLUT = 0
}

material {
	name: skybox
	shader: skybox

	cubemap: skybox = 0
}
