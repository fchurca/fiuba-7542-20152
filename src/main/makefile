CFLAGS += -std=c++11 -g
CXX += $(CFLAGS)
CXXO = $(CXX) -o $@ -c $<

all: bin/tp7542

bin/tp7542: bin/main.o bin/parser_yaml.o bin/logger.o bin/gfx.o bin/controller.o bin/board.o bin/entity.o bin/sprite_sheet.o bin/game.o
	$(CXX) -o $@ -lyaml-cpp -lSDL2 -lSDL2_image $^

bin/main.o: main.cpp
	$(CXXO)

bin/gfx.o: gfx/game_window.cpp gfx/game_window.h
	$(CXXO)

bin/logger.o: log/logger.cpp log/logger.h
	$(CXXO)

bin/parser_yaml.o: parser_yaml/parser_yaml.cpp parser_yaml/parser_yaml.h
	$(CXXO)

bin/controller.o: controllers/event_handler.cpp controllers/event_handler.h
	$(CXXO)

bin/board.o: model/board.cpp model/board.h
	$(CXXO)

bin/entity.o: model/entity.cpp model/entity.h
	$(CXXO)

bin/sprite_sheet.o: gfx/sprite_sheet.cpp gfx/sprite_sheet.h
	$(CXXO)

bin/game.o: game.cpp game.h
	$(CXXO)

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

