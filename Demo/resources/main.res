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

////////////////////////////////////////////////////////////////////////////////////

texture {
	name: citron
	src: textures/citron.png
	format: RGBA
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
}

texture {
	name: sword_N
	src: textures/sword/sword_N.png
	format: RGB
	mips: true
}

texture {
	name: sword_M
	src: textures/sword/sword_M.png
	format: RED
	mips: true
}

texture {
	name: sword_R
	src: textures/sword/sword_R.png
	format: RED
	mips: true
}

texture {
	name: sword_AO
	src: textures/sword/sword_AO.png
	format: RED
	mips: true
}

////////////////////////////////////////////////////////////////////////////////////

mesh {
	name: sword
	src: meshs/sword.ob
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
