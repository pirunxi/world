// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "BinaryStream.h"
#include "Sockets.h"
#include "Networking.h"

/**
 * 
 */

enum SessionState
{
	BEFORE_CONNECT,
	CONNECTING,
	CONNECTED,
	CLOSED,
};

class WORLD_API Session
{
protected:
	int Id;
	const char* Host;
	int Port;
	FSocket* Socket;

	int SocketRecvSize;
	int SocketSendSize;

	SessionState State;

	BinaryStream InputBuffer;
	BinaryStream OutputBuffer;

public:
	Session(const char* Host, int Port);

	virtual ~Session();

	int GetId()
	{
		return Id;
	}

	const char* GetHost()
	{
		return Host;
	}

	int GetPort()
	{
		return Port;
	}

	FSocket* GetSocket()
	{
		return Socket;
	}

	bool IsClosed()
	{
		return State == CLOSED;
	}

	virtual bool Connect() final;
	virtual void Close() final;
	virtual void Tick() final;

	virtual void OnConnect();
	virtual void OnClose(int err);
	virtual void OnRecv(int type, BinaryStream& MsgBody) = 0;
	virtual void Send(BinaryStream& bs) = 0;

protected:

private:
	static int AllocNextId()
	{
		static int NEXT_ID = 0;
		return ++NEXT_ID;
	}

	FSocket* CreateSocket();
};


DECLARE_LOG_CATEGORY_EXTERN(NetLog, Log, All)