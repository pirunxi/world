// Fill out your copyright notice in the Description page of Project Settings.

#include "Session.h"
#include "NetWork.h"
#include <string>

DEFINE_LOG_CATEGORY(NetLog);

Session::Session(const char* host, int port) : Id(AllocNextId()), Host(host), Port(port), SocketRecvSize(1024 * 128), SocketSendSize(1024 * 32),
	Socket(nullptr), State(SessionState::BEFORE_CONNECT)
{
}

FSocket* Session::CreateSocket()
{
	FSocket* socket = nullptr;

	ISocketSubsystem* SocketSubsystem = ISocketSubsystem::Get(PLATFORM_SOCKETSUBSYSTEM);

	if (SocketSubsystem != nullptr)
	{
		socket = SocketSubsystem->CreateSocket(NAME_Stream, TEXT("socket"), true);

		if (socket != nullptr)
		{
			bool Error = !socket->SetReuseAddr(true) ||
			//	!Socket->SetLinger(Linger, LingerTimeout) ||
				!socket->SetRecvErr();

			if (!Error)
			{
				Error = !socket->SetNonBlocking();
			}

			if (!Error)
			{
				int32 OutNewSize;

				if (SocketRecvSize > 0)
				{
					socket->SetReceiveBufferSize(SocketRecvSize, OutNewSize);
				}

				if (SocketSendSize > 0)
				{
					socket->SetSendBufferSize(SocketSendSize, OutNewSize);
				}
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

bool Session::Connect()
{
	if (State != BEFORE_CONNECT)
	{
		UE_LOG(NetLog, Error, TEXT("try connect session twice!"));
		return false;
	}

	State = CONNECTING;

	NetWork::GetInstance().AddSession(this);

	ISocketSubsystem* SocketSubSystem = ISocketSubsystem::Get(PLATFORM_SOCKETSUBSYSTEM);
	TSharedRef<FInternetAddr> Addr = SocketSubSystem->CreateInternetAddr(0, 0);
	FIPv4Address ipAddr;
	if (!FIPv4Address::Parse(ANSI_TO_TCHAR(Host), ipAddr))
	{
		ESocketErrors SocketError = SocketSubSystem->GetHostByName(Host, *Addr);
		if (SocketError != 0)
		{
			UE_LOG(NetLog, Error, TEXT("Unknown host:%s"), ANSI_TO_TCHAR(Host));
			Close();
			return false;
		}
	}
	else
	{
		Addr->SetIp(ipAddr.Value);
	}
	Addr->SetPort(Port);

	Socket = CreateSocket();
	if (Socket == nullptr)
	{
		UE_LOG(NetLog, Error, TEXT("create socket fail"));
		Close();
		return false;
	}

	if (!Socket->Connect(*Addr))
	{
		UE_LOG(NetLog, Error, TEXT("connect socket fail"));
		Close();
		return false;
	}
	return true;
}

void Session::Close()
{
	if (State == CLOSED)
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
	//socket_destroy(&Socket);
	OnClose(1);
}

void Session::Tick()
{
	switch (Socket->GetConnectionState())
	{
		case ESocketConnectionState::SCS_Connected:
		{
			if (State == CONNECTING)
			{
				State = CONNECTED;
				OnConnect();
			}
			{
				int ReadBuffSize = 1024 * 10;

				int ReadSize;
				if (!Socket->Recv((uint8*)InputBuffer.Reserve(ReadBuffSize), ReadBuffSize, ReadSize))
				{
					Close();
					return;
				}
				if (ReadSize > 0)
				{
					UE_LOG(NetLog, Log, TEXT("recv byets:%d"), ReadSize);
					InputBuffer.AppendWithoutCopy(ReadSize);
				}
			}
			{
				if (!OutputBuffer.empty())
				{
					int32 WriteSize;
					if (!Socket->Send((const uint8*)OutputBuffer.GetDataHead(), OutputBuffer.size(), WriteSize))
					{
						Close();
						return;
					}
					if (WriteSize > 0)
					{
						UE_LOG(NetLog, Log, TEXT("send byets:%d"), WriteSize);
						OutputBuffer.Skip(WriteSize);
					}
				}
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
