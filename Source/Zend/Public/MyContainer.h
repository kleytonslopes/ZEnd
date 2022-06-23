// Copyright Sunydark 2022.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Interfaces/MyInteractableInterface.h"
#include "MyContainer.generated.h"

class UStaticMeshComponent;
class UMyInventoryComponent;
class UMyLootBoxComponent;

UCLASS()
class ZEND_API AMyContainer : public AActor, public IMyInteractableInterface
{
	GENERATED_BODY()
	
private:
	UMyLootBoxComponent* LootBox;

protected:
    UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "My Container")
	UStaticMeshComponent* Mesh;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "My Container|Components")
	UMyInventoryComponent* Inventory;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "My Container")
	FName LootBoxName;

	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = "My Container")
	bool bIsFirstOpen = true;

	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = "My Container")
	float MaxBulk = 1.f; //V = a * a * a

	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = "My Container")
	float CurrentBulk;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "My Container")
	float DefaultWeight = 5.f;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "My Container")
	float CurrentWeight;

public:	
	AMyContainer();

protected:
	virtual void BeginPlay() override;

public:	

	void Interact_Implementation(AActor* Caller) override;

	UFUNCTION(BlueprintCallable)
	virtual void OnInteract(AActor* Caller);

	UFUNCTION(BlueprintCallable)
	bool CanPickUp() const;

private:
	void TraceLine(ECollisionChannel CollisionChannel, FVector EndLocation, FHitResult& HitResult, bool& Success, bool DrawDebug);
	void SetDefaults();
};
