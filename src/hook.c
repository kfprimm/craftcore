
#include <craftcore.h>

cc_hook_t *cc_new_hook()
{
	cc_hook_t *hook = cc_new(hook);
	hook->func = NULL;
	hook->next = NULL;
	return hook;
}

void cc_hook_add(cc_hook_t *hook, CCHOOKFUNC func)
{
	cc_hook_t *end = hook;
	while (end->next != NULL)
		end = end->next;
	end->next = cc_new_hook();
	end->next->func = func;
}

void cc_hook_run(cc_hook_t *hook, void *data)
{
	cc_hook_t *run = hook->next;
	while (run != NULL)
	{
		(run->func)(data);
		run = run->next;
	}
}
