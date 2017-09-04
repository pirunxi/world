// Fill out your copyright notice in the Description page of Project Settings.

#include "World.h"
#include "Modules/ModuleManager.h"

#include "Logger.h"

IMPLEMENT_PRIMARY_GAME_MODULE( WorldModule, World, "World" );


void WorldModule::StartupModule()
{
	Logger(Warning, TEXT("module <World> start"));
}

void WorldModule::ShutdownModule()
{
	Logger(Warning, TEXT("MODULE <World> shutdown"));
}
