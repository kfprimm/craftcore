
#include <craftcore.h>
#include <stdio.h>

// TODO: Remove
#include <GL/gl.h>
#include <GL/glu.h>

int ccContinue = TRUE;
float ccMove = 0.0f;
int ccLastMouseX = 0, ccLastMouseY = 0;
int ccMouseX = 0, ccMouseY = 0;

int ccMouseDown1 = 0;

int ccKeyW = 0, ccKeyS = 0, ccKeyA = 0, ccKeyD = 0, ccKeySpace = 0;

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
		case KEY_SPACE:ccKeySpace = 0;break;
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
		case KEY_SPACE:ccKeySpace = 1;break;		
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
		if (ev->data == 1)
			ccMouseDown1 = 1;
		break;
	case EVENT_MOUSEUP:
		if (ev->data == 2)
		{
			ccLookAround = 0;
			cc_mouse_visible(!ccLookAround);	
		}
		if (ev->data == 1)
			ccMouseDown1 = 0;
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

	//cc_entity_set_position(world.camera, 0,6,CHUNKSIZE*4);
	
	cc_hook_add(cc_event_hook, (CCHOOKFUNC)HookFunc);

	cc_world_build_chunk(&world, world.chunks[0]);
	cc_chunk_build_tree(world.chunks[0]);

	cc_world_build_chunk(&world, world.chunks[0]);
	cc_chunk_build_tree(world.chunks[0]);
	
	cc_vec3_t hit;
	cc_line_t line;
	int index;
		
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
		
		if (ccMouseDown1)
		{
			cc_camera_unproject(world.camera, width / 2, height / 2, 0.0, &line.p0);
			cc_camera_unproject(world.camera, width / 2, height / 2, 1.0, &line.p1);
			
			float t;
			if (cc_octree_line_intersection(world.chunks[0]->tree, &line, &t, (void**)&index))
			{
				int x, y, z;
				z = index / (CHUNKSIZE*CHUNKSIZE);
				y = (index - (z*CHUNKSIZE*CHUNKSIZE)) / CHUNKSIZE;
				x = (index - (z*CHUNKSIZE*CHUNKSIZE)) - (y*CHUNKSIZE);
				
				world.chunks[0]->block[x][y][z] = 0;
				
				cc_world_build_chunk(&world, world.chunks[0]);
				cc_chunk_build_tree(world.chunks[0]);
				
				cc_line_point(&line, t, &hit);
				printf("HIT: { %f, %f, %f } index = %i (%i, %i, %i)\n", hit.x, hit.y, hit.z, index, x, y, z);
			}		
			ccMouseDown1 = 0;
		}
		
		cc_entity_move(world.camera, (ccKeyD-ccKeyA), 0, (ccKeyS-ccKeyW));
		cc_render_3d();
		cc_world_render(&world, width, height);
		
		/*glDisable(GL_DEPTH_TEST);
		glPointSize(10);
		glColor3f(1.0,0.0,0.0);
		glBegin(GL_POINTS);
		glVertex3fv(&hit);
		glEnd();
		glLineWidth(1.0f);
		glColor3f(0.0,1.0,0.0);
		glBegin(GL_LINES);
		glVertex3fv(&line.p0);
		glVertex3fv(&line.p1);
		glEnd();
		glEnable(GL_DEPTH_TEST);*/
		
		cc_render_2d(width, height);
		cc_ui_render(L);
		cc_context_flip();
	}
	cc_context_close();
	return 0;
}
