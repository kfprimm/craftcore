
#include <craftcore.h>

ccHook *ccNewHook()
{
	ccHook *hook = ccNew(ccHook);
	hook->func = NULL;
	hook->next = NULL;
	return hook;
}

void ccHookAdd(ccHook *hook, CCHOOKFUNC func)
{
	ccHook *end = hook;
	while (end->next != NULL)
		end = end->next;
	end->next = ccNewHook();
	end->next->func = func;
}

void ccHookRun(ccHook *hook, void *data)
{
	ccHook *run = hook->next;
	while (run != NULL)
	{
		(run->func)(data);
		run = run->next;
	}
}
