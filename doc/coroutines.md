# Coroutines

```cpp
Coroutine co;

co.start([](Coroutine::Context const &ctx) {
	//...

	ctx.yield();

	//...

	ctx.wait(0.5f);

	//...
});

co.resume(Time::delta); // si dans frame
co.resume(); // Time::delta par defaut
co.resume(Time::fixedDelta); // si dans tick
co.resume(0.1f); // ou bien sur n'importe quel delta

if(co.isAlive()) { // arrêt de la coroutine même si elle n'est pas terminée
	co.kill();
}
```

Important : si une coroutine est détruite avant la fin de son exécution, l'API n'appellera pas les destructeurs des objets.

L'exemple suivant va créer une fuite de mémoire :
```cpp
Coroutine co;

co.start([](Coroutine::Context const &ctx) {
	Ptr<int> value = createPtr<int>(7);

	//...

	ctx.yield();

	//...
});

co.kill();
```
