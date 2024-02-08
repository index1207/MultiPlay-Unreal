// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Manager.generated.h"

#define INIT_MANAGER(name)\
if (!name##Object)												\
{																\
	if (name##Class->StaticClass())								\
	{															\
		name##Object = NewObject<U##name>(this, name##Class);	\
	}															\
	else														\
	{															\
		name##Object = name##Class.GetDefaultObject();			\
	}															\
}																\

class UNetwork;

/**
 * 
 */
UCLASS()
class MULTIPLAY_API UManager : public UGameInstance
{
	GENERATED_BODY()
public:
	UManager();
	virtual ~UManager() override;
public:
	UFUNCTION(BlueprintCallable)
	void BeginPlay();

	UFUNCTION(BlueprintCallable)
	void Tick();

	UFUNCTION(BlueprintCallable)
	void EndPlay();
	
	void ConnectToServer() const;
	void HandlePacket() const;
	void DisconnectFromServer() const;
public:
	static TObjectPtr<UNetwork> Net(const UWorld* World = GEngine->GameViewport->GetWorld());
private:
	static UManager* Instance(const UWorld* World = GEngine->GameViewport->GetWorld());
	void Initialize();
private:
	UPROPERTY(EditAnywhere)
	TSubclassOf<UNetwork> NetworkClass;
	UPROPERTY()
	TObjectPtr<UNetwork> NetworkObject;
};	
