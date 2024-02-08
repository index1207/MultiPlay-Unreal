// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "NetworkCharacter.h"
#include "PlayableCharacter.generated.h"

UCLASS()
class MULTIPLAY_API	APlayableCharacter : public ANetworkCharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	APlayableCharacter();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

	virtual bool IsMine() const override;
public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
protected:
	void MoveForward(float AxisValue);
	void MoveRight(float AxisValue);
	void BeginSprint();
	void EndSprint();
protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	class USpringArmComponent* SpringArm;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	class UCameraComponent* Camera;
protected:
	const float MOVE_SEND_DELAY = 0.2f;
	float MoveSendTimer = MOVE_SEND_DELAY;
};
