// Fill out your copyright notice in the Description page of Project Settings.

#include "LuaManager.h"

#include "Paths.h"
#include <string>

#include "Logger.h"
#include "UserData/AllUserDatas.h"
//#include "Engine.h"

#include <iostream>

LuaManager LuaManager::Instance;

LuaManager::LuaManager() : _L(nullptr)
{
}

LuaManager::~LuaManager()
{
}

void LuaManager::Start()
{
	Logger(Log, TEXT("LuaManager::Start"));


	_L = luaL_newstate();
	luaL_openlibs(_L);

	FString LuaPath = FPaths::Combine(FPaths::GameContentDir(), TEXT("Lua"), TEXT("?.lua"));
	AddSearchPath(TCHAR_TO_UTF8(*LuaPath));

	RegisterCppClasses();
	InitLuaMain();


	//GEngine->AddOnScreenDebugMessage(-1, -1, FColor::Red, TEXT("阿妹你看，上帝压狗! "));
}

void LuaManager::InitLuaMain()
{
	if (luaL_dostring(_L, "return require('main')"))
	{
		Logger(Log, TEXT("load main fail. err:%s"), UTF8_TO_TCHAR(lua_tostring(_L, -1)));
	}
	else
	{
		Logger(Log, TEXT("load main.lua succ"));
		if (lua_istable(_L, -1))
		{
			lua_getfield(_L, -1, "Init");
			if (lua_pcall(_L, 0, 0, 0))
			{
				Logger(Error, TEXT("main.Init fail. err:%s"), UTF8_TO_TCHAR(lua_tostring(_L, -1)));
			}
			else
			{
				Logger(Log, TEXT("main.Init succ"));
			}
		}
		else
		{
			Logger(Error, TEXT("main.lua should return table"));
		}
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
	Logger(Log, TEXT("LuaManager::AddSearchpath old:%s add:%s new:%s new2:%s"), UTF8_TO_TCHAR(oldPath), UTF8_TO_TCHAR(SearchPath),
		UTF8_TO_TCHAR(newPath.c_str()), UTF8_TO_TCHAR(lua_tostring(_L, -1)));
	lua_pop(_L, 2);
}

class LuaSession : public Session
{

private:
	std::function<void()> _onConnectFun;
	std::function<void(int)> _onCloseFun;
	std::function<void(int, BinaryStream&)>& _onRecvFun;

public:

	LuaSession(const char* host, int port, std::function<void()> onConnectFun, std::function<void(int)> onCloseFun, std::function<void(int, BinaryStream&)>& onRecvFun)
		: Session(host, port), _onConnectFun(onConnectFun), _onCloseFun(onCloseFun), _onRecvFun(onRecvFun)
	{

	}

	void OnConnect()
	{
		Session::OnConnect();
		_onConnectFun();
	}

	void OnClose(int err)
	{
		Session::OnClose(err);
		_onCloseFun(err);
	}

	void OnRecv(int type, BinaryStream& MsgBody)
	{
		_onRecvFun(type, MsgBody);
	}
};

Session* LuaManager::CreateSession(const char * host, int port, std::function<void()> onConnectFun, std::function<void(int)> onCloseFun, std::function<void(int, BinaryStream&)> onRecvFun)
{
	return new LuaSession(host, port, onConnectFun, onCloseFun, onRecvFun);
}

void LuaManager::Shutdown()
{
	Logger(Log, TEXT("LuaManager::Shutdown"));
	if (_L)
	{
		lua_close(_L);
		_L = nullptr;
	}
}

void LuaManager::Tick(float DeltaSeconds)
{

}



void LuaManager::RegisterCppClasses()
{
	sol::state_view lua(_L);
	UserData::RegisterAll(lua);
}

