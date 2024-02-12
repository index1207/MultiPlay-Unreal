// Fill out your copyright notice in the Description page of Project Settings.


#include "Managers/Network.h"

#include "NetworkCharacter.h"
#include "PlayableCharacter.h"
#include "Utility.h"
#include "Network/IoThread.h"
#include "Engine/World.h"
#include "Kismet/GameplayStatics.h"

UNetwork::UNetwork() : bIsConnected(false), IoThread(nullptr)
{
}

UNetwork::~UNetwork()
{
}

bool UNetwork::Connect(const net::Endpoint& EndPoint)
{
	Socket.create(net::Protocol::Tcp);
	bIsConnected = Socket.connect(EndPoint);
	if (bIsConnected)
	{
		IoThread = new ::FIoThread(&Socket);
		gen::LoginReq Pkt;
		Send(&Pkt);
	}
	return bIsConnected;
}

void UNetwork::Disconnect()
{
	if (bIsConnected)
	{
		bIsConnected = false;
		Socket.disconnect();
		Socket.close();

		delete IoThread;
	}
}

bool UNetwork::IsConnected() const
{
	return bIsConnected;
}

void UNetwork::Send(Packet* Packet) const
{
	if (Session)
		Session->Send(Packet);
}

void UNetwork::SetSession(const TSharedPtr<FSession>& NewSession)
{
	this->Session = NewSession;
}
 
TSharedPtr<FSession> UNetwork::GetSession()
{
	return Session;
}

void UNetwork::HandleSpawn(const gen::PlayerInfo& Info, bool IsMine)
{
	auto* World = GEngine->GameViewport->GetWorld();
	if (!Socket.isOpen() || Session == nullptr || World == nullptr)
		return;
	
	const auto ObjectId = Info.objectId;
	if (Players.Find(ObjectId) != nullptr)
		return;

	FVector SpawnLocation = Utility::MakeFVector(Info.status.location);
	if (IsMine)
	{
		auto* PlayerController = UGameplayStatics::GetPlayerController(this, 0);
		if (auto* Player = CastChecked<APlayableCharacter>(PlayerController->GetPawn()))
		{
			Player->SetActorLocation(SpawnLocation);
			Player->SetPlayerInfo(Info);
			
			MyPlayer = Player;
			Players.Add(Info.objectId, Player);
		}
	}
	else
	{
		auto OtherPlayer = CastChecked<ANetworkCharacter>(World->SpawnActor(OtherPlayerClass, &SpawnLocation));
		OtherPlayer->SetPlayerInfo(Info);
		Players.Add(Info.objectId, OtherPlayer);
	}
}

void UNetwork::HandleSpawn(const gen::EnterGameRes& Res)
{
	HandleSpawn(Res.player, true);
}

void UNetwork::HandleSpawn(const gen::SpawnNotify& Notify)
{	
	for (const auto& player : Notify.playerList)
		HandleSpawn(player, false);
}

void UNetwork::HandleDespawn(uint64 ObjectId)
{
	auto* World = GetWorld();
	if (World == nullptr)
		return;

	auto targetActor = Players.Find(ObjectId);
	if (targetActor == nullptr)
		return;
	
	World->DestroyActor(*targetActor);
}

void UNetwork::HandleDespawn(const gen::DespawnNotify& Notify)
{
	for (const auto& player : Notify.playerList)
	{
		HandleDespawn(player);
	}
}

void UNetwork::HandleMove(const gen::MoveRes& Packet)
{
	auto* World = GetWorld();
	if (World == nullptr)
		return;

	const auto ObjectId = Packet.info.objectId;
	const auto Player = Players.Find(ObjectId);
	if (Player == nullptr)
		return;
	
	auto PlayerPtr = *Player;
	if (PlayerPtr->GetPlayerInfo()->objectId == MyPlayer->GetPlayerInfo()->objectId)
		return;
	auto Info = Packet.info;
	// PlayerRef->SetPlayerInfo(Info);
	PlayerPtr->SetDestination(Info.status);
}