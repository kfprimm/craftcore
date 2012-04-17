
#include <craftcore.h>
#include <stdio.h>

int ccContinue = TRUE;
float ccMove = 0.0f;
int ccLastMouseX = 0, ccLastMouseY = 0;
int ccMouseX = 0, ccMouseY = 0;

int ccKeyW = 0, ccKeyS = 0, ccKeyA = 0, ccKeyD = 0;

int ccLookAround = 0;

extern ccEntity camera;

void HookFunc(ccEvent *ev)
{
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
			ccMoveMouse(ccContextWidth()/2, ccContextHeight()/2);			
			ccLookAround = 1;
			ccMouseVisible(!ccLookAround);
		}
		break;
	case EVENT_MOUSEUP:
		if (ev->data == 2)
		{
			ccLookAround = 0;
			ccMouseVisible(!ccLookAround);		
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

int main()
{
	ccEventStartup();
	ccUIStartup();
	ccLuaStartup();
			
	ccOpenContext("CraftCore", 640, 480);
	ccRenderStartup();


	
	ccEntityPosition(&camera, 0,6,CHUNKSIZE*4);
	
	ccHookAdd(ccEventHook, (CCHOOKFUNC)HookFunc);
	
	while (ccContinue)
	{
		ccPollSystem();
		
		if (ccLookAround)
		{
			int center_x = ccContextWidth()/2, center_y = ccContextHeight()/2;
			float pitch, yaw, roll;
			ccEntityRotation(&camera, &pitch, &yaw, &roll);
			pitch -= (center_y - ccMouseY)*0.5;
			yaw   += (center_x - ccMouseX)*0.5;
			
			pitch = min(max(pitch, -85), 85);
			
			ccEntityRotate(&camera, pitch, yaw, roll);
			ccMoveMouse(center_x, center_y);			
		}
		
		ccEntityMove(&camera, (ccKeyD-ccKeyA), 0, (ccKeyS-ccKeyW));
		ccRender(ccContextWidth(), ccContextHeight());
		ccRenderUI();
		ccFlip();
	}
	ccCloseContext();
	return 0;
}
