
#include <craftcore.h>

LUALIB_API int luaopen_yaml(lua_State *L);

lua_State *L;

void cc_startup_lua()
{
	L = lua_open();
	luaL_openlibs(L);
	luaopen_yaml(L);
	
	lua_newtable(L);
	lua_pushnumber(L, 1);
	lua_pushstring(L, "standard");
	lua_rawset(L, -3);
	lua_setglobal(L, "arg");
	
	if (luaL_loadfile(L, "scripts/boot.lua") == 0)
		lua_call(L, 0, 0);
	else
		fprintf(stdout, "Failed to load boot script!\n");
}
