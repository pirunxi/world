// Fill out your copyright notice in the Description page of Project Settings.



#include "WorldGameInstance.h"
#include "Ticker.h"

#include "Net/NetWork.h"
#include "LuaManager.h"
#include "Logger.h"

void UWorldGameInstance::Init()
{
	Super::Init();
	Logger(Warning, TEXT("GameInstance Init"));

	Logger(Log, TEXT("GameDir:%s GameConfigDir:%s GameContenDir:%s GameSaveDir:%s GameUserDir:%s name:%s"),
		*FPaths::GameDir(), *FPaths::GameConfigDir(), *FPaths::GameContentDir(), *FPaths::GameSavedDir(), *FPaths::GameUserDir(), UTF8_TO_TCHAR("asdfa»ÆÇ¿"));

	TickDelegatehandle = FTicker::GetCoreTicker().AddTicker(FTickerDelegate::CreateUObject(this, &UWorldGameInstance::Tick));


	NetWork::GetInstance().Start();
	LuaManager::GetInstance().Start();
}

void UWorldGameInstance::Shutdown()
{
	Super::Shutdown();
	Logger(Warning, TEXT("GameInstance Shutdown"));
	FTicker::GetCoreTicker().RemoveTicker(TickDelegatehandle);
	if (luaState != nullptr)
	{
		lua_close(luaState);
		luaState = nullptr;
		Logger(Log, TEXT("Destory luaState succ"));
	}

	NetWork::GetInstance().Shutdown();
	LuaManager::GetInstance().Shutdown();
}



bool UWorldGameInstance::Tick(float DeltaSeconds)
{
	//Logger(Log, TEXT("GameInstance Tick"));
	NetWork::GetInstance().Tick();
	LuaManager::GetInstance().Tick(DeltaSeconds);
	return true;
}
