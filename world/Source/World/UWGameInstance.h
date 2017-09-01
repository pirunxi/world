// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "lua.hpp"

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "UWGameInstance.generated.h"


/**
 * 
 */
UCLASS()
class WORLD_API UUWGameInstance : public UGameInstance
{
	GENERATED_BODY()



	void Init() override;
	void Shutdown() override;
	

public:
	lua_State* GetLuaState()
	{
		return luaState;
	}

private:
	bool Tick(float DeltaSeconds);

	FDelegateHandle TickDelegatehandle;

	lua_State* luaState;
};
