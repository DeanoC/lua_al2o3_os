#include "al2o3_platform/platform.h"
#include "lua_base5.3/lua.hpp"
#include "al2o3_os/filesystem.h"

extern lua_CFunction LuaOs_DirectoryEnumeratorInit(lua_State* L);

static char const FileSystemMetaName[] = "Al2o3.Os.filesystem";

static int createFilesystem(lua_State* L) {
	// file system doesn't have any state (all 'static' function)
	// the ud is just a dummy and no need for garbage collection
	// its a bit wasteful but easy to extend if its stops being a pure static library
	lua_newuserdata(L, sizeof(uint8_t));
	return 1;
}

static int isAbsolutePath(lua_State* L) {
	char const* path = luaL_checkstring(L, 1);
	bool absolute = Os_IsAbsolutePath(path);
	lua_pushboolean(L, absolute);
	return 1;
}

static int getCurrentPath(lua_State* L) {
	char const* path = luaL_checkstring(L, 1);
	ASSERT(false);

	return 1;
}

static int setCurrentDir(lua_State* L) {
	char const* dir = luaL_checkstring(L, 1);
	bool okay = Os_SetCurrentDir(dir);
	lua_pushboolean(L, okay);
	return 1;
}

static int fileExists(lua_State* L) {
	char const* file = luaL_checkstring(L, 1);
	bool okay = Os_FileExists(file);
	lua_pushboolean(L, okay);
	return 1;
}

static int dirExists(lua_State* L) {
	char const* dir = luaL_checkstring(L, 1);
	bool okay = Os_DirExists(dir);
	lua_pushboolean(L, okay);
	return 1;
}

static int fileCopy(lua_State* L) {
	char const* src = luaL_checkstring(L, 1);
	char const* dst = luaL_checkstring(L, 2);
	bool okay = Os_FileCopy(src, dst);
	lua_pushboolean(L, okay);
	return 1;
}

static int fileDelete(lua_State* L) {
	char const* file = luaL_checkstring(L, 1);
	bool okay = Os_FileDelete(file);
	lua_pushboolean(L, okay);
	return 1;
}

static int dirCreate(lua_State* L) {
	char const* file = luaL_checkstring(L, 1);
	bool okay = Os_DirCreate(file);
	lua_pushboolean(L, okay);
	return 1;
}

static int systemRun(lua_State* L) {
	char const* filename = luaL_checkstring(L, 1);
	auto argc  = luaL_checkinteger(L, 2);
	ASSERT(false);
//	bool okay = Os_SystemRun(filename, argc, argv );
//	lua_pushboolean(L, okay);
	return 1;
}

lua_CFunction LuaOs_FileSystemInit(lua_State* L) {
	const struct luaL_Reg filesystemFuncTable [] = {
			{"create", 	LuaOs_DirectoryEnumeratorInit(L) },
			{"isAbsolutePath", &isAbsolutePath},
//			{"splitPath", &splitPath}, // might be best to have a native Lua path system...
//			{"replaceExtension", &replaceExtension},
//			{"getParentPath", &getParentPath},
//			{"getCurrentDir", },
			{"setCurrentDir", setCurrentDir},

			{"fileExists", fileExists},
			{"dirExists", dirExists},

			{"fileCopy", fileCopy},
			{"fileDelete", fileDelete},
			{"dirCreate", },
	//		{"SystemRun", },

			{nullptr, nullptr}  /* sentinel */
	};

	luaL_newmetatable(L, FileSystemMetaName);
	lua_pushvalue(L, -1);
	lua_setfield(L, -2, "__index");
	luaL_setfuncs(L, filesystemFuncTable, 0);

	return &createFilesystem;
}

