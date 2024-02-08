// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ManagerBase.generated.h"

class UManager;
/**
 * 
 */

UCLASS(Blueprintable, BlueprintType)
class MULTIPLAY_API UManagerBase : public UObject
{
	GENERATED_BODY()
public:
	UManagerBase();
	virtual ~UManagerBase() override;
};
