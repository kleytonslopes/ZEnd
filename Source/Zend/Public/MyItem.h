// Copyright Sunydark 2022.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/MyInventoryComponent.h"
#include "Engine/DataTable.h"
#include "MyItem.generated.h"

UCLASS()
class ZEND_API AMyItem : public AActor
{
	GENERATED_BODY()
	
public:
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "Item")
	UStaticMeshComponent* Mesh;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Item")
	FName ItemName;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "Item")
	FItem ItemInfor;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "Item")
	UDataTable* ItemDataTable;

public:	
	AMyItem();

	UFUNCTION(BlueprintCallable)
	void UseItem();
protected:
	virtual void BeginPlay() override;


};
