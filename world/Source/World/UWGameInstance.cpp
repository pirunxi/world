// Fill out your copyright notice in the Description page of Project Settings.

#include "UWGameInstance.h"
#include "Ticker.h"

DECLARE_LOG_CATEGORY_EXTERN(WLog, Log, All);

void UUWGameInstance::Init()
{
	Super::Init();
	UE_LOG(WLog, Warning, TEXT("GameInstance Init"));
	TickDelegatehandle = FTicker::GetCoreTicker().AddTicker(FTickerDelegate::CreateUObject(this, &UUWGameInstance::Tick));

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
}

void UUWGameInstance::Shutdown()
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
}


bool UUWGameInstance::Tick(float DeltaSeconds)
{
	//UE_LOG(WLog, Log, TEXT("GameInstance Tick"));
	return true;
}
