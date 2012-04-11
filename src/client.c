
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
		case KEY_SPACE:
			ccLookAround = !ccLookAround;
			if (ccLookAround)
				ccMouseVisible(FALSE);
			else
				ccMouseVisible(TRUE);
				
			break;
		}
		printf("KEY UP: %i\n", ev->data);
		break;
	case EVENT_KEYDOWN:
		switch (ev->data)
		{
		case KEY_W:ccKeyW = 1;break;
		case KEY_A:ccKeyA = 1;break;
		case KEY_S:ccKeyS = 1;break;
		case KEY_D:ccKeyD = 1;break;
		}
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

	ccOpenContext("CraftCore", 800, 600);
	ccRenderStartup();
	
	ccHookAdd(ccEventHook, (CCHOOKFUNC)HookFunc);
	
	while (ccContinue)
	{
		ccPollSystem();
		
		if (ccLookAround)
		{
			ccMoveMouse(ccContextWidth()/2, ccContextHeight()/2);
			//ccEntityTurn(&camera, (ccLastMouseY - ccMouseY)*.5, (ccLastMouseX - ccMouseX)*.5, 0);
			//ccEntityTurn(&camera, 1,1,1);
			ccEntityTurn(&camera, ccKeyS-ccKeyW, 0, 0);
			ccEntityTurn(&camera, 0, ccKeyD-ccKeyA, 0);
		}
		
		//ccEntityMove(&camera, ccKeyD-ccKeyA, 0, ccKeyS-ccKeyW);
		ccRender(ccContextWidth(), ccContextHeight());
		ccFlip();
	}
	ccCloseContext();
	return 0;
}
