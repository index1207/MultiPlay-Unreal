// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "net/Socket.hpp"
#include "Session.h"

/**
 * 
 */
class MULTIPLAY_API FIoThread : FRunnable
{
public:
	FIoThread(net::Socket* socket);
	virtual ~FIoThread() override;
public:
	virtual bool Init() override;
	virtual uint32 Run() override;
	virtual void Stop() override;
	virtual void Exit() override;
public:
	void Send(Packet* packet);
private:
	FRunnableThread* Thread;
	TSharedPtr<FSession> Session;
	FCriticalSection CriticalSection;
};
