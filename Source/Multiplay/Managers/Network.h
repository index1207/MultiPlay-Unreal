// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include <span>

#include "CoreMinimal.h"
#include "ManagerBase.h"
#include "Chaos/AABB.h"
#include "generated/Protocol.gen.hpp"
#include "net/Socket.hpp"
#include "Network/Packet.h"
#include "Network.generated.h"

class FSession;
class ANetworkCharacter;
/**
 * 
 */
UCLASS()
class MULTIPLAY_API UNetwork : public UManagerBase
{
	GENERATED_BODY()
	friend class FIoThread;
public:
	UNetwork();
	virtual ~UNetwork() override;
public:
	bool Connect(const net::Endpoint& EndPoint);
	void Disconnect();
	bool IsConnected() const;
	void Send(sv::Packet* Packet) const;
public:
	void SetSession(const TSharedPtr<FSession>& NewSession);
	TSharedPtr<FSession> GetSession();
public:
	void HandleSpawn(const gen::PlayerInfo& Info, bool IsMine);
	void HandleSpawn(const gen::EnterGameRes& Res);
	void HandleSpawn(const gen::SpawnNotify& Notify);

	void HandleDespawn(uint64 ObjectId);
	void HandleDespawn(const gen::DespawnNotify& Notify);
	
	void HandleMove(const gen::MoveRes& Packet);
private:
	bool bIsConnected;
	FIoThread* IoThread;
	net::Socket Socket;
	TSharedPtr<FSession> Session;
	UPROPERTY(EditAnywhere)
	TSubclassOf<ANetworkCharacter> OtherPlayerClass;
	UPROPERTY()
	TObjectPtr<ANetworkCharacter> MyPlayer;
	UPROPERTY()
	TMap<uint64, ANetworkCharacter*> Players;
};
