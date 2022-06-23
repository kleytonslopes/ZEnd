// Copyright Sunydark 2022.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "MyInventoryComponent.h"
#include "MyInventoryManagerComponent.generated.h"

class UMyInventoryComponent;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class ZEND_API UMyInventoryManagerComponent : public UActorComponent
{
	GENERATED_BODY()

protected:
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "Inventory")
	UMyInventoryComponent* PrimaryInventory;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "Inventory")
	UMyInventoryComponent* SecondaryInventory;

public:	
	UMyInventoryManagerComponent();

protected:
	virtual void BeginPlay() override;

public:
	UFUNCTION(BlueprintCallable)
	void SetPrymaryInventory(UMyInventoryComponent* PrimaryInventoryRef);

	UFUNCTION(BlueprintCallable)
	void SetSecondaryInventory(UMyInventoryComponent* SecondaryInventoryRef);

	UFUNCTION(BlueprintCallable)
	void TransferAllItensFromTo(UMyInventoryComponent* FromInventory, UMyInventoryComponent* ToInventory);

	UFUNCTION(BlueprintCallable)
	void TransferItemFromTo(FItem ItemToTransfer, UMyInventoryComponent* FromInventory, UMyInventoryComponent* ToInventory);

	
};
