// Fill out your copyright notice in the Description page of Project Settings.

#include "LuaManager.h"

#include "Paths.h"
#include <string>


DECLARE_LOG_CATEGORY_EXTERN(WLog, Log, All);


LuaManager LuaManager::Instance;

LuaManager::LuaManager() : _L(nullptr)
{
}

LuaManager::~LuaManager()
{
}

void LuaManager::Start()
{
	UE_LOG(WLog, Log, TEXT("LuaManager::Start"));
	_L = luaL_newstate();
	luaL_openlibs(_L);

	UE_LOG(WLog, Log, TEXT("GameDir:%s GameConfigDir:%s GameContenDir:%s GameSaveDir:%s GameUserDir:%s name:%s"),
		*FPaths::GameDir(), *FPaths::GameConfigDir(), *FPaths::GameContentDir(), *FPaths::GameSavedDir(), *FPaths::GameUserDir(), UTF8_TO_TCHAR("asdfa黄强"));

	FString LuaPath = FPaths::Combine(FPaths::GameContentDir(), TEXT("Lua"), TEXT("?.lua"));
	AddSearchPath(TCHAR_TO_UTF8(*LuaPath));

	if (luaL_dostring(_L, "require 'main'"))
	{
		UE_LOG(WLog, Log, TEXT("load main fail. err:%s"), UTF8_TO_TCHAR(lua_tostring(_L, -1)));
	}
	lua_settop(_L, 0);
}

void LuaManager::AddSearchPath(const char* SearchPath)
{
	lua_getglobal(_L, "package");
	lua_getfield(_L, -1, "path");
	const char* oldPath = lua_tostring(_L, -1);
	std::string newPath = std::string(oldPath) + ";" + SearchPath;
	lua_pop(_L, 1);
	lua_pushstring(_L, newPath.c_str());
	lua_setfield(_L, -2, "path");
	
	lua_getfield(_L, -1, "path");
	UE_LOG(WLog, Log, TEXT("LuaManager::AddSearchpath old:%s add:%s new:%s new2:%s"), UTF8_TO_TCHAR(oldPath), UTF8_TO_TCHAR(SearchPath),
		UTF8_TO_TCHAR(newPath.c_str()), UTF8_TO_TCHAR(lua_tostring(_L, -1)));
	lua_pop(_L, 2);
}

void LuaManager::Shutdown()
{
	UE_LOG(WLog, Log, TEXT("LuaManager::Shutdown"));
	if (_L)
	{
		lua_close(_L);
		_L = nullptr;
	}
}

void LuaManager::Tick(float DeltaSeconds)
{
}

