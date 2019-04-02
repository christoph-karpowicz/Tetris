export EMCC_DEBUG=1

app.js: app.cpp
	emcc --bind --std=c++11 -c app.cpp
	emcc --bind --std=c++11 -c Game.cpp
	emcc --bind --std=c++11 -c GameObject.cpp
	emcc --bind --std=c++11 -c Square.cpp
	emcc --bind --std=c++11 -c Figure.cpp
	emcc --bind --std=c++11 -c FigureCell.cpp
	emcc --bind app.o Game.o GameObject.o Square.o Figure.o FigureCell.o -s WASM=1 -o app.js -s "EXTRA_EXPORTED_RUNTIME_METHODS=['addOnPostRun','cwrap']"

clean:
	rm *.js *.wasm *.o
