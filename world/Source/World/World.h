// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

class WorldModule : public IModuleInterface
{
public:
	virtual bool IsGameModule() const override
	{
		return true;
	}

	virtual void StartupModule() override;

	virtual void ShutdownModule() override;
};
