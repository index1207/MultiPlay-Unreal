// Fill out your copyright notice in the Description page of Project Settings.


#include "IoThread.h"

#include "Managers/Network.h"
#include "Multiplay/Managers/Manager.h"


FIoThread::FIoThread(net::Socket* socket) : Session(MakeShared<::Session>(socket))
{
	UManager::Net()->SetSession(Session);
	Session->OnConnected();
	Thread = FRunnableThread::Create(this, TEXT("Net I/O Thread"));
}

FIoThread::~FIoThread()
{
	if(Thread)
	{
		Thread->Kill();
		delete Thread;
	}
}

bool FIoThread::Init()
{
	return FRunnable::Init();
}

uint32 FIoThread::Run()
{
	while(Session->GetHandle()->isOpen())
	{	
		auto n = Session->GetHandle()->receive(Session->m_buffer);
		if (n > 0)
			Session->OnReceive(Session->m_buffer, n);
		else
		{
			GEngine->AddOnScreenDebugMessage(-1, 10, FColor::Yellow, TEXT("RECV 0"));
			Session->OnDisconnected();
			break;
		}
	}
	return 0;
}

void FIoThread::Stop()
{
	FRunnable::Stop();
}

void FIoThread::Exit()
{
	FRunnable::Exit();
}

void FIoThread::Send(Packet* packet)
{
	FScopeLock Lock(&CriticalSection);
	Session->Send(packet);
}
