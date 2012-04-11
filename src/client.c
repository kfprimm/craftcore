
#include <craftcore.h>
#include <stdio.h>

int ccContinue = TRUE;

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
		}
		printf("KEY UP: %i\n", ev->data);
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
		ccRender(ccContextWidth(), ccContextHeight());
		ccFlip();
	}
	ccCloseContext();
	return 0;
}
