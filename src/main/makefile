CFLAGS += -std=c++11 -g
CXX += $(CFLAGS)

all: bin/tp7542

bin/tp7542: bin/main.o bin/parser_yaml.o bin/logger.o bin/gfx.o bin/controller.o bin/board.o bin/entity.o
	#TODO: ordenar game, board
	$(CXX) -o $@ -lyaml-cpp -lSDL2 -lSDL2_image bin/main.o bin/parser_yaml.o bin/logger.o bin/gfx.o bin/controller.o bin/board.o bin/entity.o game.cpp

bin/main.o: main.cpp
	$(CXX) -o $@ -c main.cpp

bin/gfx.o: gfx/game_window.cpp gfx/game_window.h
	$(CXX) -o $@ -c gfx/game_window.cpp

bin/logger.o: log/logger.cpp log/logger.h
	$(CXX) -o $@ -c log/logger.cpp

bin/parser_yaml.o: parser_yaml/parser_yaml.cpp parser_yaml/parser_yaml.h
	$(CXX) -o $@ -c parser_yaml/parser_yaml.cpp

bin/controller.o: controllers/event_handler.cpp controllers/event_handler.h
	$(CXX) -o $@ -c controllers/event_handler.cpp

bin/board.o: model/board.cpp model/board.h
	$(CXX) -o $@ -c model/board.cpp

bin/entity.o: model/entity.cpp model/entity.h
	$(CXX) -o $@ -c model/entity.cpp

clean: cleanobj
	rm -f bin/tp7542

cleanobj:
	rm -rf bin/*.o

force: clean all

run: all
	bin/tp7542

cppcheck:
	cppcheck `find . -name '*.cpp'`

.PHONY: clean cleanobj all force cppcheck

