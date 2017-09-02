// Fill out your copyright notice in the Description page of Project Settings.

#include "Session.h"
#include "Common/TcpSocketBuilder.h"
#include "NetWork.h"

DEFINE_LOG_CATEGORY(NetLog);

Session::Session(FString host, int port) : Id(AllocNextId()), Host(host), Port(port), SocketRecvSize(1024 * 128), SocketSendSize(1024 * 32),
	Socket(nullptr), State(SessionState::BEFORE_CONNECT)
{
}

bool Session::Connect()
{
	if (State != BEFORE_CONNECT)
	{
		UE_LOG(NetLog, Error, TEXT("try connect session twice!"));
		return false;
	}

	NetWork::GetInstance().AddSession(this);
	if (!FIPv4Address::Parse(Host, Ipv4Addr))
	{
		auto SubSystem = ISocketSubsystem::Get(PLATFORM_SOCKETSUBSYSTEM);
		auto addr = SubSystem->CreateInternetAddr(0, 0);
		auto err = SubSystem->GetHostByName(TCHAR_TO_ANSI(*Host), *addr);
		if (err != 0)
		{
			UE_LOG(NetLog, Error, TEXT("Unknown Host:%s err:%d"), *Host, int(err));
			return false;
		}

		uint32 ip;
		addr->GetIp(ip);
		Ipv4Addr = FIPv4Address(ip);
		UE_LOG(NetLog, Log, TEXT("Resolve Host:%s IP:%s"), *Host, *Ipv4Addr.ToString());
	}
	
	Socket = CreateSocket();
	if (Socket == nullptr)
	{
		UE_LOG(NetLog, Error, TEXT("Create Socket fail"));
		Close();
		return false;
	}

	UE_LOG(NetLog, Log, TEXT("Connect Host:%s Ip:%s Port:%d"), *Host, *Ipv4Addr.ToString(), Port);
	if (!Socket->Connect(*FIPv4Endpoint(Ipv4Addr, Port).ToInternetAddr()))
	{
		UE_LOG(NetLog, Error, TEXT("Socket Connect fail"));
		Close();
		return false;
	}

	State = CONNECTING;
	return true;
}

void Session::Close()
{
	if (State >= CLOSED)
	{
		UE_LOG(NetLog, Error, TEXT("try close not connect or closed session!"));
		return;
	}
	State = CLOSED;
	if (Socket != nullptr)
	{
		ISocketSubsystem::Get(PLATFORM_SOCKETSUBSYSTEM)->DestroySocket(Socket);
		Socket = nullptr;
	}
	OnClose(1);
}

void Session::Tick()
{
	Socket->GetConnectionState();
	switch (Socket->GetConnectionState())
	{
		case ESocketConnectionState::SCS_Connected:
		{
			if (State == CONNECTING)
			{
				State = CONNECTED;
				OnConnect();
			}
			uint32 size;
			if (Socket->HasPendingData(size))
			{
				//Socket->Recv();
			}
			break;
		}
		case ESocketConnectionState::SCS_ConnectionError:
		{
			Close();
			break;
		}
	}
}

void Session::OnConnect()
{
	UE_LOG(NetLog, Log, TEXT("Session:%d Connect succ"), Id);
}

void Session::OnClose(int err)
{
	UE_LOG(NetLog, Log, TEXT("Session:%d Close"), Id);
}

Session::~Session()
{
}

FSocket * Session::CreateSocket()
{
	FSocket* socket = nullptr;

	ISocketSubsystem* SocketSubsystem = ISocketSubsystem::Get(PLATFORM_SOCKETSUBSYSTEM);

	if (SocketSubsystem != nullptr)
	{
		socket = SocketSubsystem->CreateSocket(NAME_Stream, TEXT("Session"), false);
		if (socket != nullptr)
		{
			bool Error = !socket->SetReuseAddr(true) ||
				//!Socket->SetLinger(Linger, LingerTimeout) ||
				!socket->SetRecvErr();

			//if (!Error)
			//{
			//	Error = !socket->Bind(*FIPv4Endpoint(Ipv4Addr, Port).ToInternetAddr());
			//}

			if (!Error)
			{
				Error = !socket->SetNonBlocking(true);
			}

			if (!Error)
			{
				int32 OutNewSize;
				socket->SetReceiveBufferSize(SocketRecvSize, OutNewSize);
				socket->SetSendBufferSize(SocketSendSize, OutNewSize);
			}

			if (Error)
			{
				SocketSubsystem->DestroySocket(socket);
				socket = nullptr;
			}
		}
	}
	return socket;
}
