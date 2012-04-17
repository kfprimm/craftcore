
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

int main()
{
	cc_startup_event();
	cc_startup_ui();
	cc_world_init(&world);
	
	cc_startup_lua();

	//luaL_dofile(L, "mods/standard/scripts/worldgen.lua");

	cc_context_open("CraftCore", 640, 480);
	cc_startup_render();

	cc_entity_set_position(world.camera, 0,6,CHUNKSIZE*4);
	
	cc_hook_add(cc_event_hook, (CCHOOKFUNC)HookFunc);
	
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
		cc_world_render(&world, width, height);
		cc_ui_render();
		cc_context_flip();
	}
	cc_context_close();
	return 0;
}
