# Signaux

## Utilisation

```cpp
// création d'un nouveau type de signal
// indiquer dans quelle pile sera ajoutés les signaux
Signals::registerType<MySignalType>(Signals::Stack::BEFORE_TICK);

// écoute d'un type de signal
Listener listener = Signals::listen<MySignalType>([] (MySignalType const &signal){
	//code
});

// écoute d'un type de signal via une méthode
class MyClass {
public:
	MyClass() {
		listener = Signals::listen<MySignalType>(&MyClass::listenMethod, this);
	}

	listenMethod(MySignalType const &signal) {
		// code
	}

private:
	Listener listener;
}

// emission d'un signal
Signals::emit<MySignalType>(Args...);
```

## Les Stacks

```
BEFORE_TICK
BEFORE_FRAME
```

## Signaux intégrés

```
WindowResizeSignal
KeyPressSignal
KeyReleaseSignal
```
