shader {
	name: loadingAnimation
	src: shaders/loadingAnimation.glsl
}

shader {
	name: textureTest
	src: shaders/textureTest.glsl
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
