all: bin/tp7542

bin/tp7542: main.cpp bin/parser_yaml.o bin/logger.o bin/gfx.o
	$(CXX) -o $@ main.cpp -lyaml-cpp -lSDL2 bin/parser_yaml.o bin/logger.o bin/gfx.o

bin/gfx.o: gfx/game_window.cpp gfx/game_window.h
	$(CXX) -o $@ -c gfx/game_window.cpp

bin/logger.o: log/logger.cpp log/logger.h
	$(CXX) -o $@ -c log/logger.cpp

bin/parser_yaml.o: parser_yaml/parser_yaml.cpp parser_yaml/parser_yaml.h
	$(CXX) -o $@ -c parser_yaml/parser_yaml.cpp

clean: cleanobj
	rm -f bin/tp7542

cleanobj:
	rm -rf bin/*.o

force: clean all

run: all
	bin/tp7542

.PHONY: clean cleanobj all force

