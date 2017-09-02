// Fill out your copyright notice in the Description page of Project Settings.

#include "NetWork.h"
#include <algorithm>

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
	UE_LOG(NetLog, Log, TEXT("NetWork.AddSession session:%d host:%s port:%d"),
		session->GetId(), *session->GetHost(), session->GetPort());
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
			delete ss;
			SomeToRemove = true;
			UE_LOG(NetLog, Log, TEXT("NetWork.Tick remove session:%d"), ss->GetId());
		}
	}
	if (SomeToRemove)
	{
		ActiveSessiones.erase(std::remove_if(ActiveSessiones.begin(), ActiveSessiones.end(), [](Session* session) { return session->IsClosed(); }), ActiveSessiones.end());
	}
}

NetWork::~NetWork()
{
}

class TestSession : public Session
{
public:
	using Session::Session;

	void OnRecv(int type, BinaryStream& MsgBody) override
	{

	}

	void Send(BinaryStream& bs) override
	{

	}

	void OnClose(int err)
	{
		Session::OnClose(err);
	}
};

void NetWork::Start()
{

	auto ss = new TestSession(TEXT("dev-proxy.oa.com"), 8080);
	ss->Connect();
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

