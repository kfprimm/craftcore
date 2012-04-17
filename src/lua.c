
#include <craftcore.h>

LUALIB_API int luaopen_yaml(lua_State *L);
LUALIB_API int luaopen_noise(lua_State *L);

lua_State *L;

static int l_load_atlas(lua_State *L)
{
	cc_atlas_t *atlas = cc_load_atlas(lua_tostring(L, 1));
	if (atlas)
		lua_pushlightuserdata(L, atlas);
	else
		lua_pushnil(L);
	return 1;
}

static int l_atlas_texture(lua_State *L)
{
	cc_texture_t *texture = cc_atlas_texture(lua_touserdata(L, 1), lua_tointeger(L, 2), lua_tointeger(L, 3), lua_tointeger(L, 4), lua_tointeger(L, 5));
	if (texture)
		lua_pushlightuserdata(L, texture);
	else
		lua_pushnil(L);
	return 1;
}

static int l_texture_draw(lua_State *L)
{
	cc_texture_draw(lua_touserdata(L, 1), lua_tonumber(L, 2), lua_tonumber(L, 3));
	return 0;
}

static int l_context_size(lua_State *L)
{
	int width, height;
	cc_context_size(&width, &height);
	lua_pushnumber(L, width);
	lua_pushnumber(L, height);
	return 2;
}

void cc_startup_lua()
{
	L = lua_open();
	luaL_openlibs(L);
	luaopen_yaml(L);
	luaopen_noise(L);
	
	lua_newtable(L);
	lua_pushnumber(L, 1);
	lua_pushstring(L, "standard");
	lua_rawset(L, -3);
	lua_setglobal(L, "arg");
	
	lua_newtable(L);
	lua_pushcfunction(L, l_load_atlas);
	lua_setfield(L, -2, "loadAtlas");
	lua_pushcfunction(L, l_atlas_texture);
	lua_setfield(L, -2, "atlasTexture");
	lua_pushcfunction(L, l_texture_draw);
	lua_setfield(L, -2, "drawTexture");
	lua_pushcfunction(L, l_context_size);
	lua_setfield(L, -2, "contextSize");
	lua_setglobal(L, "cc");

	if (luaL_loadfile(L, "scripts/boot.lua") == 0)
		lua_call(L, 0, 0);
	else
		fprintf(stdout, "Failed to load boot script!\n");
}
