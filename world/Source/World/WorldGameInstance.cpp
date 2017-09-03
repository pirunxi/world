// Fill out your copyright notice in the Description page of Project Settings.



#include "WorldGameInstance.h"

#include "Net/NetWork.h"
#include "Ticker.h"

void UWorldGameInstance::Init()
{
	Super::Init();
	UE_LOG(WLog, Warning, TEXT("GameInstance Init"));
	TickDelegatehandle = FTicker::GetCoreTicker().AddTicker(FTickerDelegate::CreateUObject(this, &UWorldGameInstance::Tick));

	luaState = luaL_newstate();
	if (luaState == nullptr)
	{
		UE_LOG(WLog, Error, TEXT("NEW lua state fail"));
	}
	else
	{
		UE_LOG(WLog, Log, TEXT("NEW lua state succ"));
		luaL_openlibs(luaState);
	}

	NetWork::GetInstance().Start();
}

void UWorldGameInstance::Shutdown()
{
	Super::Shutdown();
	UE_LOG(WLog, Warning, TEXT("GameInstance Shutdown"));
	FTicker::GetCoreTicker().RemoveTicker(TickDelegatehandle);
	if (luaState != nullptr)
	{
		lua_close(luaState);
		luaState = nullptr;
		UE_LOG(WLog, Log, TEXT("Destory luaState succ"));
	}

	NetWork::GetInstance().Shutdown();
}



bool UWorldGameInstance::Tick(float DeltaSeconds)
{
	//UE_LOG(WLog, Log, TEXT("GameInstance Tick"));
	NetWork::GetInstance().Tick();
	return true;
}
