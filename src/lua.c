
#include <craftcore.h>

LUALIB_API int luaopen_yaml(lua_State *L);

lua_State *L;

static int lua_ccLoadAtlas(lua_State *L)
{
	ccAtlas *atlas = ccLoadAtlas(lua_tostring(L, 1));
	if (atlas)
		lua_pushlightuserdata(L, atlas);
	else
		lua_pushnil(L);
	return 1;
}

static int lua_ccAtlasTexture(lua_State *L)
{
	ccTexture *texture = ccAtlasTexture(lua_touserdata(L, 1), lua_tointeger(L, 2), lua_tointeger(L, 3), lua_tointeger(L, 4), lua_tointeger(L, 5));
	if (texture)
		lua_pushlightuserdata(L, texture);
	else
		lua_pushnil(L);
	return 1;
}

static int lua_ccDrawTexture(lua_State *L)
{
	ccDrawTexture(lua_touserdata(L, 1), lua_tonumber(L, 2), lua_tonumber(L, 3));
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
	luaopen_yaml(L);
	
	lua_newtable(L);
	lua_pushnumber(L, 1);
	lua_pushstring(L, "standard");
	lua_rawset(L, -3);
	lua_setglobal(L, "arg");
	
	lua_newtable(L);
	lua_pushcfunction(L, lua_ccLoadAtlas);
	lua_setfield(L, -2, "loadAtlas");
	lua_pushcfunction(L, lua_ccAtlasTexture);
	lua_setfield(L, -2, "atlasTexture");
	lua_pushcfunction(L, lua_ccDrawTexture);
	lua_setfield(L, -2, "drawTexture");
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
