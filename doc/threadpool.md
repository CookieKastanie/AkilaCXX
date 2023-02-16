## Threadpool

```cpp
Threadpool::submit([]() {
	// execution dans un thread
}, ()[] {
	// execution dans le thread principal
});
```

```cpp
// /!\ non implémenté
Threadpool::dispatch<5, 1, 1>([](unsigned int x, unsigned int y, unsigned int z) {
	// execution dans un thread
});
```
