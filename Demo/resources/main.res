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

material {
	name: loadingScreen
	shader: loadingAnimation
}

material {
	name: textureTest
	shader: textureTest
}
