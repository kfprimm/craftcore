
#include <craftcore.h>

lua_State *L;

static int lua_ccLoadAtlas(lua_State *L)
{
	lua_pushlightuserdata(L, ccLoadAtlas(lua_tostring(L, 1)));
	return 1;
}

static int lua_ccAtlasImage(lua_State *L)
{
	lua_pushlightuserdata(L, ccAtlasImage(lua_touserdata(L, 1), lua_tointeger(L, 2), lua_tointeger(L, 3), lua_tointeger(L, 4), lua_tointeger(L, 5)));
	return 1;
}

static int lua_ccDrawImage(lua_State *L)
{
	ccDrawImage(lua_touserdata(L, 1), lua_tonumber(L, 2), lua_tonumber(L, 3));
	return 0;
}

static int lua_ccContextWidth(lua_State *L)
{
	lua_pushnumber(L, ccContextWidth());
	return 1;
}

static int lua_ccContextHeight(lua_State *L)
{
	lua_pushnumber(L, ccContextHeight());
	return 1;
}

void ccLuaStartup()
{
	L = lua_open();
	luaL_openlibs(L);
	
	lua_newtable(L);
	lua_pushnumber(L, 1);
	lua_pushstring(L, "standard");
	lua_rawset(L, -3);
	lua_setglobal(L, "arg");
	
	lua_newtable(L);
	lua_pushcfunction(L, lua_ccLoadAtlas);
	lua_setfield(L, -2, "loadAtlas");
	lua_pushcfunction(L, lua_ccAtlasImage);
	lua_setfield(L, -2, "atlasImage");
	lua_pushcfunction(L, lua_ccDrawImage);
	lua_setfield(L, -2, "drawImage");
	lua_pushcfunction(L, lua_ccContextWidth);
	lua_setfield(L, -2, "contextWidth");
	lua_pushcfunction(L, lua_ccContextHeight);
	lua_setfield(L, -2, "contextHeight");
	lua_setglobal(L, "cc");

	if (luaL_loadfile(L, "scripts/boot.lua") == 0)
		lua_call(L, 0, 0);
	else
		fprintf(stdout, "Failed to load boot script!\n");
}
