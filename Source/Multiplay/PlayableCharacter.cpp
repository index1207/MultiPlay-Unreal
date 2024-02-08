// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayableCharacter.h"
#include <Camera/CameraComponent.h>
#include <Components/CapsuleComponent.h>

#include "Utility.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "generated/Protocol.gen.hpp"
#include "Managers/Manager.h"
#include "Managers/Network.h"

// Sets default values
APlayableCharacter::APlayableCharacter()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	
	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("Spring Arm"));
	SpringArm->SetupAttachment(GetCapsuleComponent());
	SpringArm->bEnableCameraLag = true;
	SpringArm->CameraLagSpeed = 8.5;
	SpringArm->bUsePawnControlRotation = true;
	
	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Main Camera"));
	Camera->SetupAttachment(SpringArm, USpringArmComponent::SocketName);
	Camera->bUsePawnControlRotation = false;

	IsMoving = false;
}

// Called when the game starts or when spawned
void APlayableCharacter::BeginPlay()
{
	Super::BeginPlay();
}

void APlayableCharacter::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);
}

bool APlayableCharacter::IsMine() const
{
	return true;
}

// Called every frame
void APlayableCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	MoveSendTimer -= DeltaTime;
	if (MoveSendTimer <= 0)
	{
		MoveSendTimer = MOVE_SEND_DELAY;

		if (IsMoving)
		{
			gen::MoveReq MovePacket;
			MovePacket.status = GetPlayerInfo()->status;
			UManager::Net()->Send(&MovePacket);
		}
	}
}

// Called to bind functionality to input
void APlayableCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAxis("MoveForward", this, &APlayableCharacter::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &APlayableCharacter::MoveRight);
	PlayerInputComponent->BindAxis("Turn", this, &APawn::AddControllerYawInput);
	PlayerInputComponent->BindAxis("LookUp", this, &APawn::AddControllerPitchInput);

	PlayerInputComponent->BindAction("Sprint", IE_Pressed, this, &APlayableCharacter::BeginSprint);
	PlayerInputComponent->BindAction("Sprint", IE_Released, this, &APlayableCharacter::EndSprint);
}

void APlayableCharacter::MoveForward(const float AxisValue)
{
	if (Controller && AxisValue != 0.0f)
	{
		const auto Rotation = Controller->GetControlRotation();
		const auto YawRotation = FRotator(0, Rotation.Yaw, 0);

		const auto Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
		AddMovementInput(Direction, AxisValue);
	}
}

void APlayableCharacter::MoveRight(float AxisValue)
{
	if (Controller && AxisValue != 0.0f)
	{
		const auto Rotation = Controller->GetControlRotation();
		const auto YawRotation = FRotator(0, Rotation.Yaw, 0);

		const auto Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
		AddMovementInput(Direction, AxisValue);
	}
}

void APlayableCharacter::BeginSprint()
{
	GetCharacterMovement()->MaxWalkSpeed = 800;
}

void APlayableCharacter::EndSprint()
{
	GetCharacterMovement()->MaxWalkSpeed = 400;
}

