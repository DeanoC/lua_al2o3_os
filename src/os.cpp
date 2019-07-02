#include "al2o3_platform/platform.h"
#include "lua_base5.3/lua.hpp"

extern lua_CFunction LuaOs_FileSystemInit(lua_State* L);

static char const OsMetaName[] = "Al2o3.Os";


AL2O3_EXTERN_C int LuaAL2O3Os_Open(lua_State* L) {

	const struct luaL_Reg osLib [] = {
			{"filesystem", LuaOs_FileSystemInit(L) },
			{nullptr, nullptr}  /* sentinel */
	};

	luaL_newlib(L, osLib);
	return 1;
}