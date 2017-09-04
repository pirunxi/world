// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

DECLARE_LOG_CATEGORY_EXTERN(WLog, Log, All);

#define Logger(Verbosity, Format, ...) UE_LOG(WLog, Verbosity, Format, ##__VA_ARGS__)

// GEngine->AddOnScreenDebugMessage(-1, -1, FColor::Red, TEXT("°¢ÃÃÄã¿´£¬ÉÏµÛÑ¹¹·! "));