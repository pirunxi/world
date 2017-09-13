// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "sol.hpp"
#include "Net/BinaryStream.h"
#include "Net/Session.h"

/**
 * 
 */
class LuaManager
{
public:
	static LuaManager& GetInstance() { return Instance;  }

	void Start();
	void Shutdown();

	void Tick(float DeltaSeconds);

	void AddSearchPath(const char* SearchPath);

	Session* CreateSession(const char* host, int port, std::function<void()> onConnectFun, std::function<void(int)> onCloseFun, std::function<void(int, BinaryStream&)> onRecvFun);
	LuaManager();
	~LuaManager();
private:

	void RegisterCppClasses();
	void InitLuaMain();

	lua_State* _L;

	static LuaManager Instance;
};
