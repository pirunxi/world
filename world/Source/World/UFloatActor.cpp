// Fill out your copyright notice in the Description page of Project Settings.

#include "UFloatActor.h"
#include "lua.hpp"
#include "WorldGameInstance.h"

DECLARE_LOG_CATEGORY_EXTERN(WLog, Log, All);

// Sets default values
AUFloatActor::AUFloatActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AUFloatActor::BeginPlay()
{
	Super::BeginPlay();

	UWorldGameInstance* gs = Cast<UWorldGameInstance>(GetGameInstance());
	if (gs != nullptr)
	{
		lua_State* L = gs->GetLuaState();
		if (luaL_dostring(gs->GetLuaState(), "local a = 5; local b = 10; return a + b") == 0)
		{
			UE_LOG(WLog, Warning, TEXT("lua script result:%s"), ANSI_TO_TCHAR(lua_tostring(L, -1)));
		}
		else
		{
			UE_LOG(WLog, Warning, TEXT("lua script error:%s"), ANSI_TO_TCHAR(lua_tostring(L, -1)));
		}
	}
	TotalTime = 0;
}

// Called every frame
void AUFloatActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	auto NewTotalTime = TotalTime + DeltaTime;
	int R = 20;
	auto Position = GetActorLocation();
	Position.X += R * (FMath::Cos(NewTotalTime) - FMath::Cos(TotalTime));
	Position.Y += R * (FMath::Sin(NewTotalTime) - FMath::Sin(TotalTime));

	SetActorLocation(Position);
	TotalTime = NewTotalTime;
}

