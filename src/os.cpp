#include "al2o3_platform/platform.h"
#include "lua_base5.3/lua.hpp"

extern int LuaOs_FileSystemInit(lua_State* L);

static char const OsMetaName[] = "Al2o3.Os";


AL2O3_EXTERN_C int LuaOs_Open(lua_State* L) {

	const struct luaL_Reg osLib [] = {
			{nullptr, nullptr}  /* sentinel */
	};

	luaL_newlib(L, osLib);

	LuaOs_FileSystemInit(L);
	lua_setfield(L, -2, "filesystem");

	return 1;
}