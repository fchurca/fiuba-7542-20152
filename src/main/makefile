LDLIBS += -lyaml-cpp -lSDL2 -lSDL2_image -lSDL2_ttf -Lvendor/lib

CFLAGS += -std=c++11 -g -Ivendor/include

CXX += $(CFLAGS)
CXXT = $(CXX) -o $@
CXXO = $(CXXT) -c $<

all: bin/tp7542

bin/tp7542: bin/standalone_main.o bin/geometry.o bin/logger.o bin/game_window.o bin/game_timer.o bin/controller.o bin/board.o bin/entity.o bin/sprite_sheet.o bin/players_list.o bin/resources_list.o bin/selection_menu.o bin/command_menu.o bin/selection_controller.o bin/health_bar.o bin/game.o bin/mixins.o bin/entity_factory.o bin/player.o bin/budget.o bin/budget_line.o bin/abstract_client.o bin/remote_client.o bin/command.o bin/minimap.o bin/generic_parser.o bin/client_parser.o bin/graphics_parser.o bin/ruleset_parser.o bin/scenario_parser.o bin/server_parser.o bin/server.o bin/posix_socket.o bin/isoview.o bin/remote_board.o bin/menu.o bin/chat.o bin/socket.o
	${CXXT} $^ $(LDLIBS)
	cp $@ .

bin/standalone_main.o: standalone/main.cpp server/remote_client.h
	${CXXO}

bin/isoview.o: gfx/isoview.cpp gfx/isoview.h
	${CXXO}

bin/menu.o: gfx/menu.cpp gfx/menu.h
	${CXXO}

bin/game_window.o: gfx/game_window.cpp gfx/game_window.h model/game.h model/board.h model/entity.h
	${CXXO}

bin/sprite_sheet.o: gfx/sprite_sheet.cpp gfx/sprite_sheet.h
	${CXXO}

bin/minimap.o: gfx/minimap.cpp gfx/minimap.h
	${CXXO}

bin/chat.o: gfx/chat.cpp gfx/chat.h
	${CXXO}

bin/players_list.o: gfx/players_list.cpp gfx/players_list.h
	${CXXO}

bin/resources_list.o: gfx/resources_list.cpp gfx/resources_list.h
	${CXXO}

bin/selection_menu.o: gfx/selection_menu.cpp gfx/selection_menu.h 
	${CXXO}

bin/command_menu.o: gfx/command_menu.cpp gfx/command_menu.h
	${CXXO}

bin/selection_controller.o: gfx/selection_controller.cpp gfx/selection_controller.h
	${CXXO}

bin/health_bar.o: gfx/health_bar.cpp gfx/health_bar.h
	${CXXO}

bin/game_timer.o: gfx/game_timer.cpp gfx/game_timer.h
	${CXXO}

bin/logger.o: log/logger.cpp log/logger.h
	${CXXO}

bin/generic_parser.o: parser_yaml/generic_parser.cpp parser_yaml/generic_parser.h
	${CXXO}

bin/client_parser.o: parser_yaml/client_parser.cpp parser_yaml/client_parser.h parser_yaml/generic_parser.h
	${CXXO}

bin/graphics_parser.o: parser_yaml/graphics_parser.cpp parser_yaml/graphics_parser.h parser_yaml/generic_parser.h
	${CXXO}

bin/ruleset_parser.o: parser_yaml/ruleset_parser.cpp parser_yaml/ruleset_parser.h parser_yaml/generic_parser.h
	${CXXO}

bin/scenario_parser.o: parser_yaml/scenario_parser.cpp parser_yaml/scenario_parser.h parser_yaml/generic_parser.h
	${CXXO}

bin/server_parser.o: parser_yaml/server_parser.cpp parser_yaml/server_parser.h parser_yaml/generic_parser.h
	${CXXO}

bin/controller.o: controllers/event_handler.cpp controllers/event_handler.h
	${CXXO}

bin/mixins.o: model/mixins.cpp model/mixins.h
	${CXXO}

bin/geometry.o: model/geometry.cpp model/geometry.h
	${CXXO}

bin/board.o: model/board.cpp model/board.h model/entity.h model/entity_factory.h model/player.h
	${CXXO}

bin/remote_board.o: client/remote_board.cpp client/remote_board.h model/board.h
	${CXXO}

bin/entity.o: model/entity.cpp model/entity.h model/geometry.h
	${CXXO}

bin/entity_factory.o: model/entity_factory.cpp model/entity_factory.h model/entity.h
	${CXXO}

bin/game.o: model/game.cpp model/game.h
	${CXXO}

bin/player.o: model/player.cpp model/player.h
	${CXXO}

bin/budget.o: model/budget.cpp model/budget.h
	${CXXO}

bin/budget_line.o: model/budget_line.cpp model/budget_line.h
	${CXXO}

bin/abstract_client.o: model/abstract_client.cpp model/abstract_client.h model/player.h
	${CXXO}

bin/remote_client.o: server/remote_client.cpp server/remote_client.h
	${CXXO}

bin/command.o: model/command.cpp model/command.h model/board.h
	${CXXO}

bin/server.o: server/server.cpp server/server.h model/game.h
	${CXXO}

bin/socket.o: socket/socket.cpp socket/socket.h
	${CXXO}

bin/posix_socket.o: socket/posix/posixsocket.cpp socket/posix/posixsocket.h socket/socket.h
	${CXXO}

clean: cleanobj
	rm -f bin/tp7542_standalone

cleanobj:
	rm -rf bin/*.o

force: clean all

run: all
	./tp7542

cppcheck:
	cppcheck `find . -name '*.cpp'`

.PHONY: clean cleanobj all force cppcheck

