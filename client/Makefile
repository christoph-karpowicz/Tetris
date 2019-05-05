export EMCC_DEBUG=1

app.js: app.cpp
	emcc --bind --std=c++11 -c app.cpp
	emcc --bind --std=c++11 -c Game.cpp
	emcc --bind --std=c++11 -c Shape.cpp
	emcc --bind --std=c++11 -c GameObject.cpp
	emcc --bind --std=c++11 -c Square.cpp
	emcc --bind --std=c++11 -c SquareSet.cpp
	emcc --bind --std=c++11 -c SquareSetRotationFunctions.cpp
	emcc --bind --std=c++11 -c GameToStringParser.cpp
	emcc --bind --std=c++11 -c Logger.cpp
	emcc --bind app.o Game.o Shape.o GameObject.o Square.o SquareSet.o SquareSetRotationFunctions.o GameToStringParser.o Logger.o -s WASM=1 -o app.js -s "EXTRA_EXPORTED_RUNTIME_METHODS=['addOnPostRun','cwrap']"

clean:
	rm *.js *.wasm *.o
