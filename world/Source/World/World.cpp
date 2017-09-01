// Fill out your copyright notice in the Description page of Project Settings.

#include "World.h"
#include "Modules/ModuleManager.h"

IMPLEMENT_PRIMARY_GAME_MODULE( WorldModule, World, "World" );

DEFINE_LOG_CATEGORY(WLog);

void WorldModule::StartupModule()
{
	UE_LOG(WLog, Warning, TEXT("module <World> start"));
}

void WorldModule::ShutdownModule()
{
	UE_LOG(WLog, Warning, TEXT("MODULE <World> shutdown"));
}
