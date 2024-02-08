// Fill out your copyright notice in the Description page of Project Settings.


#include "NetworkCharacter.h"

#include "PlayableCharacter.h"
#include "Utility.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"

// Sets default values
ANetworkCharacter::ANetworkCharacter()
{
	GetCapsuleComponent()->InitCapsuleSize(42, 96);

	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->RotationRate = FRotator(0, 500, 0);

	GetCharacterMovement()->JumpZVelocity = 700;
	GetCharacterMovement()->AirControl = 0.35f;
	GetCharacterMovement()->MinAnalogWalkSpeed = 20;
	GetCharacterMovement()->BrakingDecelerationWalking = 2000;

	SetMoveSpeed(400);
	
	PlayerInfo = new gen::PlayerInfo;
	DestInfo = new gen::Status;
}

ANetworkCharacter::~ANetworkCharacter()
{
	delete PlayerInfo;
	delete DestInfo;
}

const gen::PlayerInfo* ANetworkCharacter::GetPlayerInfo() const
{
	return PlayerInfo;
}

void ANetworkCharacter::SetPlayerInfo(const gen::PlayerInfo& Info)
{
	*PlayerInfo = Info;

	const auto Location = Utility::MakeFVector(PlayerInfo->status.location);
	SetActorLocationAndRotation(Location, FRotator(0, PlayerInfo->status.yaw, 0));
}

void ANetworkCharacter::SetDestination(const gen::Status& DestLocation) const
{
	*DestInfo = DestLocation;
}

gen::EMoveState ANetworkCharacter::GetMoveState() const
{
	return PlayerInfo->status.state;
}

void ANetworkCharacter::SetMoveState(gen::EMoveState State) const
{
	if (GetMoveState() != State)
		PlayerInfo->status.state = State;
}

// Called when the game starts or when spawned
void ANetworkCharacter::BeginPlay()
{
	Super::BeginPlay();

	*DestInfo = Utility::MakeStatus(GetActorLocation(), GetActorRotation().Yaw, GetMoveSpeed());
}

void ANetworkCharacter::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
	PlayerInfo->status = Utility::MakeStatus(GetActorLocation(), GetControlRotation().Yaw, GetMoveSpeed());

	if (!IsMine())
	{
		const auto Location = GetActorLocation();
		const auto Dest = Utility::MakeFVector(DestInfo->location);

		FVector MoveDir = Dest - Location;
		const auto DistToDest = MoveDir.Length();
		MoveDir.Normalize();

		float MoveDist = (MoveDir * DestInfo->speed * DeltaSeconds).Length();
		MoveDist = FMath::Min(MoveDist, DistToDest);
		auto NextLocation = Location + MoveDir * MoveDist;
		
		SetActorLocationAndRotation(NextLocation, FRotator(0, DestInfo->yaw, 0));
	}
}

bool ANetworkCharacter::IsMine() const
{
	return false;
}

void ANetworkCharacter::SetMoveSpeed(float Value) const
{
	GetCharacterMovement()->MaxWalkSpeed = Value;
}

float ANetworkCharacter::GetMoveSpeed() const
{
	return GetCharacterMovement()->MaxWalkSpeed;
}
