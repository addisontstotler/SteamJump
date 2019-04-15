CFLAGS =  $(shell sdl2-config --cflags)  -O2 
LIBS = -lSDL2_image -lSDL2_mixer -lSDL2_ttf $(shell sdl2-config --libs) -lm

main: score.h SDLfunctions.h player.h player.cpp airGauge.h airGauge.cpp ListNode.h ListNode.cpp List.h List.h map.h  main.cpp
	g++ $(CFLAGS) player.cpp map.cpp airGauge.cpp ListNode.cpp List.cpp main.cpp -o steamJump $(LIBS)
