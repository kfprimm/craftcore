
#include <craftcore.h>
#include <noise/noise.h>

//double noise1(double arg);
static int l_noise2(lua_State *L)
{
	float vec[2] = { 0.0, 0.0 };
	lua_pushnumber(L, 1);
	lua_gettable(L, 1);
	lua_pushnumber(L, 2);
	lua_gettable(L, 1);
	vec[0] = lua_tonumber(L, -2);
	vec[1] = lua_tonumber(L, -1);
	lua_pop(L, 2);
	lua_pushnumber(L, noise2(vec));
	return 1;
}
//float noise3(float vec[3]);

LUALIB_API int luaopen_noise(lua_State *L)
{
	lua_register(L, "noise2", l_noise2);
	return 1;
}
