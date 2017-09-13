// Fill out your copyright notice in the Description page of Project Settings.

#include "NetWork.h"
#include <algorithm>

#include "Logger.h"

NetWork NetWork::Instance;

NetWork::NetWork()
{
}

NetWork & NetWork::GetInstance()
{
	return Instance;
}

void NetWork::AddSession(Session * session)
{
	ActiveSessiones.push_back(session);
	Logger(Log, TEXT("NetWork.AddSession session:%d host:%s port:%d"),
		session->GetId(), ANSI_TO_TCHAR(session->GetHost()), session->GetPort());
}

void NetWork::Tick()
{
	bool SomeToRemove = false;
	for (auto ss : ActiveSessiones)
	{
		if (!ss->IsClosed())
		{
			ss->Tick();
		}
		else
		{
			SomeToRemove = true;
			Logger(Log, TEXT("NetWork.Tick remove session:%d"), ss->GetId());
		}
	}
	if (SomeToRemove)
	{
		ActiveSessiones.erase(std::remove_if(ActiveSessiones.begin(), ActiveSessiones.end(),
			[](Session* session)
		{
			if (session->IsClosed())
			{
				delete session;
				return true;
			}
			else
			{
				return false;
			}
		}),
			ActiveSessiones.end());
	}
}

NetWork::~NetWork()
{
}

void NetWork::Start()
{

	//auto ss = new TestSession(TEXT("dev-proxy.oa.com"), 8080);
	//auto ss = new LuaSession("127.0.0.1", 1218);
	//ss->Connect();
}

void NetWork::Shutdown()
{
	for (auto ss : ActiveSessiones)
	{
		if (!ss->IsClosed())
		{
			ss->Close();
		}
		delete ss;
	}
	ActiveSessiones.clear();
}

