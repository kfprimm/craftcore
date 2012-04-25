
#include <craftcore.h>
#include <stdio.h>

int ccContinue = TRUE;
float ccMove = 0.0f;
int ccLastMouseX = 0, ccLastMouseY = 0;
int ccMouseX = 0, ccMouseY = 0;

int ccKeyW = 0, ccKeyS = 0, ccKeyA = 0, ccKeyD = 0;

int ccLookAround = 0;

extern cc_entity_t camera;

void HookFunc(cc_event_t *ev)
{
	int width, height;
	switch (ev->id)
	{
	case EVENT_CLOSEBUTTON:
		ccContinue = FALSE;
		break;
	case EVENT_KEYUP:
		switch (ev->data)
		{
		case KEY_ESCAPE:
			ccContinue = FALSE;
			break;
		case KEY_W:ccKeyW = 0;break;
		case KEY_A:ccKeyA = 0;break;
		case KEY_S:ccKeyS = 0;break;
		case KEY_D:ccKeyD = 0;break;
		}
		//printf("KEY UP: %i\n", ev->data);
		break;
	case EVENT_KEYDOWN:
		switch (ev->data)
		{
		case KEY_W:ccKeyW = 1;break;
		case KEY_A:ccKeyA = 1;break;
		case KEY_S:ccKeyS = 1;break;
		case KEY_D:ccKeyD = 1;break;
		}
		break;
	case EVENT_MOUSEDOWN:
		if (ev->data == 2)
		{
			ccMouseX = ev->x;
			ccMouseY = ev->y;
			cc_context_size(&width, &height);
			cc_mouse_move(width/2, height/2);			
			ccLookAround = 1;
			cc_mouse_visible(!ccLookAround);
		}
		break;
	case EVENT_MOUSEUP:
		if (ev->data == 2)
		{
			ccLookAround = 0;
			cc_mouse_visible(!ccLookAround);		
		}
		break;
	case EVENT_MOUSEMOVE:
		ccLastMouseX = ccMouseX;
		ccLastMouseY = ccMouseY;
		ccMouseX = ev->x;
		ccMouseY = ev->y;
		break;
	}
}

cc_world_t world;

lua_State *L;
LUALIB_API int luaopen_yaml(lua_State *L);

int main()
{
	cc_startup_event();
	cc_startup_ui();
	cc_startup_network();
	cc_world_init(&world);
	
	L = lua_open();
	luaL_openlibs(L);
	luaopen_yaml(L);
	
	lua_newtable(L);
	lua_pushnumber(L, 1);
	lua_pushstring(L, "standard");
	lua_rawset(L, -3);
	lua_setglobal(L, "arg");
	
	luaL_loadfile(L, "scripts/init.lua");
	lua_call(L,0,0);

	lua_getglobal(L, "cm");
	lua_pushlightuserdata(L, &world);
	lua_setfield(L, -2, "world");
	
	if (luaL_loadfile(L, "scripts/boot.lua") == 0)
		lua_call(L, 0, 0);
	else
		fprintf(stdout, "Failed to load boot script!\n");

	luaL_loadfile(L, "mods/standard/scripts/worldgen.lua");
	lua_call(L,0,0);

	lua_getglobal(L, "cm");
	lua_getfield(L, -1, "meta");
	lua_getfield(L, -1, "name");

	cc_context_open(lua_tostring(L, -1), 640, 480);

	lua_pop(L, 3);

	cc_startup_render();

	cc_entity_set_position(world.camera, 0,6,CHUNKSIZE*4);
	
	cc_hook_add(cc_event_hook, (CCHOOKFUNC)HookFunc);

	cc_world_chunk_build(&world, world.chunks[0]);
	
	while (ccContinue)
	{
		cc_system_poll();
		int width, height;
		cc_context_size(&width, &height);		
		
		if (ccLookAround)
		{
			float pitch, yaw, roll;
			int center_x = width/2, center_y = height/2;
						
			cc_entity_get_rotation(world.camera, &pitch, &yaw, &roll);
			pitch -= (center_y - ccMouseY)*0.5;
			yaw   += (center_x - ccMouseX)*0.5;
			
			pitch = min(max(pitch, -85), 85);
			
			cc_entity_set_rotation(world.camera, pitch, yaw, roll);
			cc_mouse_move(center_x, center_y);			
		}
		
		cc_entity_move(world.camera, (ccKeyD-ccKeyA), 0, (ccKeyS-ccKeyW));
		cc_render_3d();
		cc_world_render(&world, width, height);
		cc_render_2d(width, height);
		cc_ui_render();
		cc_context_flip();
	}
	cc_context_close();
	return 0;
}
