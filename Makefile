
LUA_FLAGS = `pkg-config --cflags luajit`
LUA_LIBS  = `pkg-config --libs luajit`

C_FLAGS = -O3 -Wall -std=c99 -Isrc $(LUA_FLAGS)
LIBS    = -lX11 -lGLEW -lGL -lGLU $(LUA_LIBS)

craftcore: src/client.o src/rendering.o src/entity.o src/event.o src/hook.o src/system.o src/math.o
	$(CC) $(C_FLAGS) -o $@ $^ $(LIBS)

src/%.o: src/%.c src/craftcore.h src/keycodes.h
	$(CC) $(C_FLAGS) -c -o $@ $<
	
clean:
	rm -f craftcore
	rm -f src/*.o
