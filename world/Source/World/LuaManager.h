// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "sol.hpp"
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
private:
	LuaManager();
	~LuaManager();

	void RegisterCppClasses();
	void InitLuaMain();

	lua_State* _L;

	static LuaManager Instance;
};
