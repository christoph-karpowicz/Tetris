export EMCC_DEBUG=1

server: server.cpp
	g++ server.cpp Server.cpp DB.cpp Callback.cpp Router.cpp -levent -l sqlite3 -o ../app