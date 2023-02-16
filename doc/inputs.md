# Inputs

## Utilisation

```cpp
if(Inputs::isPressed(Inputs::Key::A) {
	//...
}

Listener l = Signals::listen<KeyPressSignal>(KeyPressSignal const &s) {
	if(s.key == Inputs::Key::A) {
		//...
	}
}


Vec2 pos = Inputs::getMousePosition();
Vec2 vel = Inputs::getMouseVelocity();
```

## Écoute avec les signaux

```cpp
Listener listener = Signals::listen<KeyPressSignal>([] (KeyPressSignal const &s) {
	if(s.key == Inputs::Key::ESC) {
		Window::close();
	}
});
```
