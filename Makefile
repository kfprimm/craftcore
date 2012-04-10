
C_FLAGS = -Wall -std=c99 -Isrc
LIBS    = -lX11 -lGL -lGLU

craftcore: src/client.o src/rendering.o src/event.o src/hook.o src/system.o src/craftcore.h
	$(CC) $(C_FLAGS) -o $@ $^ $(LIBS)

src/%.o: src/%.c
	$(CC) $(C_FLAGS) -c -o $@ $^
	
clean:
	rm craftcore
	rm src/*.o
