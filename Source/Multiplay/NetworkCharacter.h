// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "generated/Struct.gen.hpp"

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "NetworkCharacter.generated.h"

UCLASS()
class MULTIPLAY_API ANetworkCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ANetworkCharacter();
	virtual ~ANetworkCharacter() override;
public:
	FORCEINLINE const gen::PlayerInfo* GetPlayerInfo() const;
	FORCEINLINE void SetPlayerInfo(const gen::PlayerInfo& PlayerInfo);
	
	FORCEINLINE void SetDestination(const gen::Status& DestLocation) const;

	FORCEINLINE gen::EMoveState GetMoveState() const;
	FORCEINLINE void SetMoveState(gen::EMoveState State) const;
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaSeconds) override;
protected:
	virtual bool IsMine() const;
	void SetMoveSpeed(float Value) const;
	float GetMoveSpeed() const;
protected:
	gen::PlayerInfo* PlayerInfo;
	gen::Status* DestInfo;
};
