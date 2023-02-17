# Audio

```cpp
Ref<AudioSource> source = Resources::get<AudioSource>("mySource");

AudioEmitter emitter{source};

emitter.setPosition({1, 0, 5});

// jouer le sons :
emitter.play();
// ou
Audio::detath(emitter); // detach permet de lancer des instances,
// pour simultanément jouer le même son plusieurs fois
```
