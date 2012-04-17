
#include <craftcore.h>

cc_hook_t *cc_event_hook;

void cc_emit_event(int id, int data, int x, int y)
{
	cc_event_t event;
	event.id   = id;
	event.data = data;
	event.x    = x;
	event.y    = y;
	cc_hook_run(cc_event_hook, &event);
}

void cc_startup_event()
{
	cc_event_hook = cc_new_hook();
}
