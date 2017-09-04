// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Session.h"
#include <vector>

#include "CoreMinimal.h"


/**
 * 
 */
class WORLD_API NetWork
{
public:
	static NetWork& GetInstance();

	void AddSession(Session* session);
	void RemoveSession(Session* session);

	void Tick();
	virtual ~NetWork();

	void Start();
	void Shutdown();

private:
	NetWork();
	static NetWork Instance;

	std::vector<Session*> ActiveSessiones;
};
