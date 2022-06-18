// Copyright Sunydark 2022.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Interfaces/MyInteractableInterface.h"
#include "Components/MyInventoryComponent.h"
#include "Engine/DataTable.h"
#include "MyItemDrop.generated.h"

class UStaticMeshComponent;

UCLASS()
class ZEND_API AMyItemDrop : public AActor, public IMyInteractableInterface
{
	GENERATED_BODY()
	
public:
	UPROPERTY(BlueprintReadWrite, VisibleDefaultsOnly)
	UStaticMeshComponent* Mesh;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "Item")
	UDataTable* ItemDataTable;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Item", meta = (ExposeOnSpawn = true))
	TArray<FName> DefaultItemsNames;

	UPROPERTY(BlueprintReadWrite, Category = "Item")
	TArray<FItem> Items;

	UPROPERTY(BlueprintReadWrite, Category = "Item")
	FString ItemId;

public:	
	AMyItemDrop();

protected:
	virtual void BeginPlay() override;

public:
	void Interact_Implementation(AActor* Caller) override;

	UFUNCTION(BlueprintCallable)
	virtual void OnInteract(AActor* Caller);

	UFUNCTION(BlueprintCallable)
	void AddItem(FItem ItemToAdd);

	UFUNCTION(BlueprintCallable)
	void AddItemByName(FName ItemName);

	UFUNCTION(BlueprintCallable)
	void CreateDefaults();
};
