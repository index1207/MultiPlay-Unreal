// Fill out your copyright notice in the Description page of Project Settings.


#include "Manager.h"

#include "Kismet/GameplayStatics.h"
#include "Managers/Network.h"
#include "Network/Session.h"

UManager::UManager() : NetworkObject(nullptr)
{
}

UManager::~UManager()
{
}

void UManager::BeginPlay()
{
	Initialize();
	ConnectToServer();
}

void UManager::Tick()
{
	HandlePacket();
}

void UManager::EndPlay()
{
	DisconnectFromServer();
}

void UManager::ConnectToServer() const
{
	if (!NetworkObject->Connect(net::Endpoint(net::IpAddress::parse("192.168.0.63"), 9999)))
		GEngine->AddOnScreenDebugMessage(-1, 5, FColor::Red, TEXT("Connect failed."));
}

void UManager::HandlePacket() const
{
	if (NetworkObject->IsConnected())
		NetworkObject->GetSession()->Flush();
}

void UManager::DisconnectFromServer() const
{
	if (NetworkObject->IsConnected())
	{
		gen::LeaveGameReq LeaveGameReq;
		NetworkObject->GetSession()->Send(&LeaveGameReq);
	}
}

TObjectPtr<UNetwork> UManager::Net(const UWorld* World)
{
	return Instance(World)->NetworkObject;
}

UManager* UManager::Instance(const UWorld* World)
{
	if (World == nullptr)
		return nullptr;
	auto manager = CastChecked<UManager>(UGameplayStatics::GetGameInstance(World));
	if (manager != nullptr)
	{
		manager->Initialize();
		return manager;
	}
#if WITH_EDITOR
	UE_LOG(LogTemp, Error, TEXT("Invalid Game Instance"));
#endif
	
	auto instance = NewObject<UManager>();
	instance->Initialize();
	return instance;
}

void UManager::Initialize()
{
	INIT_MANAGER(Network)
}

