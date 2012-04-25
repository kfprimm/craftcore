
LUA_FLAGS = `pkg-config --cflags luajit`
LUA_LIBS  = `pkg-config --libs luajit`

ENET_FLAGS = `pkg-config --cflags libenet `
ENET_LIBS  = `pkg-config --libs libenet`

YAML_FLAGS = `pkg-config --cflags yaml-0.1`
YAML_LIBS  = `pkg-config --libs yaml-0.1`

C_FLAGS = -g -Wl,-E -Wall -std=c99 -Isrc $(LUA_FLAGS) $(ENET_FLAGS) $(YAML_FLAGS)
LIBS    = -lX11 -lIL -lGLEW -lGL -lGLU $(LUA_LIBS) $(ENET_LIBS) $(YAML_LIBS)

IMPORT_DIR = mods/standard/assets

OBJECTS = src/ui.o src/network.o src/world.o src/camera.o src/entity.o src/chunk.o src/block.o src/render.o \
					src/event.o src/hook.o src/system.o src/math.o src/noise/noise.o src/yaml/lyaml.o src/yaml/b64.o
					
HEADERS = src/craftcore.h src/keycodes.h src/noise/noise.h

all: craftcore craftcore-server

craftcore: src/client.o $(OBJECTS)
	$(CC) -g $(C_FLAGS) -o $@ $^ $(LIBS)

craftcore-server: src/server.o $(OBJECTS)
	$(CC) -g $(C_FLAGS) -o $@ $^ $(LIBS)

release:
	$(CC) -O3 $(C_FLAGS) -o $@ $^ $(LIBS)
	$(CC) -O3 $(C_FLAGS) -o $@ $^ $(LIBS)
	
%.o: %.c $(HEADERS)
	$(CC) $(C_FLAGS) -c -o $@ $<

tests/%: tests/%.c
	$(CC) $(C_FLAGS) -o $@ $^ $(LIBS)

mc-textures:
	cd $(IMPORT_DIR) && unzip -f ~/.minecraft/bin/minecraft.jar
	cp $(IMPORT_DIR)/terrain.png $(IMPORT_DIR)/../textures
	rm -rf $(IMPORT_DIR)/*.class

clean:
	rm -f craftcore
	rm -f craftcore-server
	rm -f src/*.o
	rm -f src/*/*.o
	
.PHONY: tests release
