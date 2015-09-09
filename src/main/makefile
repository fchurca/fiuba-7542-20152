all: bin/tp7542

bin/tp7542: bin/main.o bin/parser_yaml.o bin/logger.o bin/gfx.o bin/controller.o
	#TODO: ordenar game, board, boardFactory
	$(CXX) -o $@ -lyaml-cpp -lSDL2 -lSDL2_image bin/main.o bin/parser_yaml.o bin/logger.o bin/gfx.o bin/controller.o game.cpp board/board.cpp board/boardFactory.cpp

bin/main.o: main.cpp
	$(CXX) -o $@ -c main.cpp

bin/gfx.o: gfx/game_window.cpp gfx/game_window.h
	$(CXX) -o $@ -c gfx/game_window.cpp

bin/logger.o: log/logger.cpp log/logger.h
	$(CXX) -o $@ -c log/logger.cpp

bin/parser_yaml.o: parser_yaml/parser_yaml.cpp parser_yaml/parser_yaml.h
	$(CXX) -o $@ -c parser_yaml/parser_yaml.cpp

bin/controller.o: controllers/event_handler.cpp
	$(CXX) -o $@ -c controllers/event_handler.cpp

clean: cleanobj
	rm -f bin/tp7542

cleanobj:
	rm -rf bin/*.o

force: clean all

run: all
	bin/tp7542

.PHONY: clean cleanobj all force

