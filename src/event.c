
#include <craftcore.h>

ccHook *ccEventHook;

void ccEmitEvent(int id, int data, int x, int y)
{
	ccEvent event;
	event.id   = id;
	event.data = data;
	event.x    = x;
	event.y    = y;
	ccHookRun(ccEventHook, &event);
}

void ccEventStartup()
{
	ccEventHook = ccNewHook();
}
