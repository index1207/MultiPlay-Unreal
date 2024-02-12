// Fill out your copyright notice in the Description page of Project Settings.


#include "NetworkCharacter.h"

#include "PlayableCharacter.h"
#include "Utility.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"

// Sets default values
ANetworkCharacter::ANetworkCharacter()
{
	
	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);

	// Don't rotate when the controller rotates. Let that just affect the camera.
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	// Configure character movement
	GetCharacterMovement()->bOrientRotationToMovement = true; // Character moves in the direction of input...	
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 500.0f, 0.0f); // ...at this rotation rate

	// Note: For faster iteration times these variables, and many more, can be tweaked in the Character Blueprint
	// instead of recompiling to adjust them
	GetCharacterMovement()->JumpZVelocity = 700.f;
	GetCharacterMovement()->AirControl = 0.35f;
	GetCharacterMovement()->MaxWalkSpeed = 500.f;
	GetCharacterMovement()->MinAnalogWalkSpeed = 20.f;
	GetCharacterMovement()->BrakingDecelerationWalking = 2000.f;

	GetCharacterMovement()->bRunPhysicsWithNoController = true;

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

	SetStatus(DestInfo->state, DestInfo->yaw);
	GetCharacterMovement()->MaxWalkSpeed = DestLocation.speed;
}

gen::EMoveState ANetworkCharacter::GetMoveState() const
{
	return PlayerInfo->status.state;
}

void ANetworkCharacter::SetStatus(const gen::EMoveState State, const float YawValue) const
{
	if (GetMoveState() != State)
		PlayerInfo->status.state = State;
	
	PlayerInfo->status.yaw = YawValue;
}

// Called when the game starts or when spawned
void ANetworkCharacter::BeginPlay()
{
	Super::BeginPlay();

	*DestInfo = Utility::MakeStatus(GetActorLocation(), GetActorRotation().Yaw, GetMoveSpeed());

	SetStatus(gen::MOVE_STATE_IDLE, GetActorRotation().Yaw);
}

void ANetworkCharacter::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
	PlayerInfo->status = Utility::MakeStatus(GetActorLocation(), GetControlRotation().Yaw, GetMoveSpeed());

	if (!IsMine())
	{
		auto Dest = Utility::MakeFVector(DestInfo->location);
		auto State = DestInfo->state;
		if (State == gen::MOVE_STATE_RUN)
		{
			SetActorRotation(FRotator(0, DestInfo->yaw, 0));
			AddMovementInput(GetActorForwardVector());
		}
		// else if (State == gen::MOVE_STATE_IDLE)
		// {
		// 	FVector Location = GetActorLocation();
		//
		// 	FVector MoveDir = (Dest - Location);
		// 	const float DistToDest = MoveDir.Length();
		// 	MoveDir.Normalize();
		//
		// 	float MoveDist = (MoveDir * DestInfo->speed * DeltaSeconds).Length();
		// 	MoveDist = FMath::Min(MoveDist, DistToDest);
		// 	if (MoveDist > 3)
		// 	{
		// 		SetActorLocation(Location + (MoveDir * 100 * DeltaSeconds));
		// 	}
		// }
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
