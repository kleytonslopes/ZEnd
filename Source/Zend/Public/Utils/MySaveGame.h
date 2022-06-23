// Copyright Sunydark 2022.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SaveGame.h"
#include "Components/MyInventoryComponent.h"
#include "MySaveGame.generated.h"

class AMyCharacter;
class UWorld;

USTRUCT(BlueprintType)
struct FPlayerSaveGame
{
	GENERATED_BODY()
public:
	FPlayerSaveGame()
	{
		PlayerLocation = FVector::ZeroVector;
		PlayerRotation = FRotator::ZeroRotator;
		PlayerHealth = 0.f;
		PlayerThirst = 0.f;
	}

	UPROPERTY(BlueprintReadWrite)
	FVector PlayerLocation;

	UPROPERTY(BlueprintReadWrite)
	FRotator PlayerRotation;

	UPROPERTY(BlueprintReadWrite)
	float PlayerHealth;

	UPROPERTY(BlueprintReadWrite)
	float PlayerThirst;

	UPROPERTY(BlueprintReadWrite)
	TArray<FItemDataSaveGame> PlayerItens;
};

USTRUCT(BlueprintType)
struct FContainerSaveGame
{
	GENERATED_BODY()

public:
	FContainerSaveGame()
	{

	}

	UPROPERTY(BlueprintReadWrite)
	FString ContainerId;

	UPROPERTY(BlueprintReadWrite)
	TArray<FItemDataSaveGame> ContainerItens;
};

UCLASS()
class ZEND_API UMySaveGame : public USaveGame
{
	GENERATED_BODY()
	
public:
	UPROPERTY(BlueprintReadWrite, VisibleAnywhere, Category = "Save Game")
	FString SaveSlotName;
	
	UPROPERTY(BlueprintReadWrite, VisibleAnywhere, Category = "Save Game")
	int32 UserIndex;

	UPROPERTY(BlueprintReadWrite, VisibleAnywhere, Category = "Save Game")
	FPlayerSaveGame PlayerData;

public:
	UMySaveGame();

	UFUNCTION(BlueprintCallable)
	void SetPlayerSaveData(AMyCharacter* MyCharacter);

	UFUNCTION(BlueprintCallable)
	void SaveCurrentGame(UWorld* World);
	UFUNCTION(BlueprintCallable)
	void LoadCurrentGame(UWorld* World);
};
