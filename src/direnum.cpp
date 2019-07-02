#include "al2o3_platform/platform.h"
#include "lua_base5.3/lua.hpp"
#include "lua_base5.3/utils.h"
#include "al2o3_os/filesystem.h"

static char const DirectoryEnumMetaName[] = "Al2o3.Os.DirectoryEnum";

// create the null directory enum user data return on the lua state
static Os_DirectoryEnumeratorHandle* direnumud_create(lua_State *L) {
	// allocate a pointer and push it onto the stack
	auto ud = (Os_DirectoryEnumeratorHandle*)lua_newuserdata(L, sizeof(Os_DirectoryEnumeratorHandle*));
	if(ud == nullptr) return nullptr;

	*ud = nullptr;
	luaL_getmetatable(L, DirectoryEnumMetaName);
	lua_setmetatable(L, -2);
	return ud;
}

static int direnumud_gc (lua_State *L) {
	auto handle = *(Os_DirectoryEnumeratorHandle*)luaL_checkudata(L, 1, DirectoryEnumMetaName);
	if (handle) Os_DirectoryEnumeratorDestroy(handle);

	return 0;
}

static int directoryEnumeratorNext(lua_State *L) {
	auto handle = *(Os_DirectoryEnumeratorHandle*)luaL_checkudata(L, lua_upvalueindex(1), DirectoryEnumMetaName);
	LUA_ASSERT(handle, L, "directoryEnumeratorNext error");
	auto item = Os_DirectoryEnumeratorSyncNext(handle);
	if(item) {
		lua_pushstring(L, item->filename);
		lua_pushboolean(L, item->directory);
		return 2;
	} else {
		return 0;
	}

}

static int createDirectoryEnumerator(lua_State* L) {
	char const* path = luaL_checkstring(L, 1);
	auto ud = direnumud_create(L);
	*ud = Os_DirectoryEnumeratorCreate(path);
	LUA_ASSERT(*ud, L, "Directory Enumerator create failed");
	lua_pushcclosure(L, &directoryEnumeratorNext, 1);
	return 1;
}

lua_CFunction LuaOs_DirectoryEnumeratorInit(lua_State* L) {
	static const struct luaL_Reg direnumFuncTable [] = {
			{"__gc", &direnumud_gc },
			{nullptr, nullptr}  /* sentinel */
	};

	luaL_newmetatable(L, DirectoryEnumMetaName);
	lua_pushvalue(L, -1);
	lua_setfield(L, -2, "__index");
	luaL_setfuncs(L, direnumFuncTable, 0);

	return &createDirectoryEnumerator;
}
