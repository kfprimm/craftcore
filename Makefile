
LUA_FLAGS = `pkg-config --cflags luajit`
LUA_LIBS  = `pkg-config --libs luajit`

ENET_FLAGS = `pkg-config --cflags libenet `
ENET_LIBS  = `pkg-config --libs libenet`

C_FLAGS = -g -O3 -Wall -std=c99 -Isrc $(LUA_FLAGS) $(ENET_FLAGS)
LIBS    = -g -lX11 -lIL -lGLEW -lGL -lGLU $(LUA_LIBS) $(ENET_LIBS)

OBJECTS = src/client.o src/world.o src/entity.o src/chunk.o src/event.o src/hook.o src/system.o src/math.o src/noise.o
HEADERS = src/craftcore.h src/keycodes.h src/noise.h

craftcore: $(OBJECTS)
	$(CC) $(C_FLAGS) -o $@ $^ $(LIBS)

src/%.o: src/%.c $(HEADERS)
	$(CC) $(C_FLAGS) -c -o $@ $<
	
clean:
	rm -f craftcore
	rm -f src/*.o
