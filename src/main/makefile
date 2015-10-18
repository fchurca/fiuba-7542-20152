CFLAGS += -std=c++11 -g
CXX += $(CFLAGS)
CXXO = $(CXX) -o $@ -c $<

all: bin/tp7542_standalone

bin/tp7542_standalone: bin/standalone_main.o bin/geometry.o bin/parser_yaml.o bin/logger.o bin/game_window.o bin/game_timer.o bin/controller.o bin/board.o bin/entity.o bin/sprite_sheet.o bin/game.o bin/entity_factory.o bin/player.o bin/abstract_client.o bin/remote_client.o
	$(CXX) -o $@ -lyaml-cpp -lSDL2 -lSDL2_image $^

bin/standalone_main.o: standalone/main.cpp
	${CXXO}

bin/geometry.o: model/geometry.cpp model/geometry.h
	${CXXO}

bin/game_window.o: gfx/game_window.cpp gfx/game_window.h model/game.h model/board.h model/entity.h
	${CXXO}

bin/game_timer.o: gfx/game_timer.cpp gfx/game_timer.h
	${CXXO}

bin/logger.o: log/logger.cpp log/logger.h
	${CXXO}

bin/parser_yaml.o: parser_yaml/parser_yaml.cpp parser_yaml/parser_yaml.h
	${CXXO}

bin/controller.o: controllers/event_handler.cpp controllers/event_handler.h
	${CXXO}

bin/board.o: model/board.cpp model/board.h model/entity.h model/entity_factory.h model/player.h
	${CXXO}

bin/entity.o: model/entity.cpp model/entity.h model/geometry.h
	${CXXO}

bin/entity_factory.o: model/entity_factory.cpp model/entity_factory.h model/entity.h
	${CXXO}

bin/sprite_sheet.o: gfx/sprite_sheet.cpp gfx/sprite_sheet.h
	${CXXO}

bin/game.o: model/game.cpp model/game.h
	${CXXO}

bin/player.o: model/player.cpp model/player.h
	${CXXO}

bin/abstract_client.o: model/abstract_client.cpp model/abstract_client.h
	${CXXO}

bin/remote_client.o: remote_client/remote_client.cpp remote_client/remote_client.h
	${CXXO}

clean: cleanobj
	rm -f bin/tp7542_standalone

cleanobj:
	rm -rf bin/*.o

force: clean all

run: all
	bin/tp7542_standalone

cppcheck:
	cppcheck `find . -name '*.cpp'`

.PHONY: clean cleanobj all force cppcheck

