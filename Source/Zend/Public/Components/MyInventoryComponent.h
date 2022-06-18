// Copyright Sunydark 2022.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Engine/DataTable.h"
#include "MyInventoryComponent.generated.h"

class UTexture2D;

USTRUCT(BlueprintType)
struct FItem : public FTableRowBase
{
	GENERATED_BODY()

public:
	FItem()
	{
		Id = "";
		Code = "";
		Name = FName("");
		Description = FText::FromString("");
		Icon = nullptr;
		Mesh = nullptr;
		DropMesh = nullptr;
		Durability = 1.f;
		MaxDurability = 1.f;
		DurabilityPerUse = 1.f;
	}

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	FString Id;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	FString Code;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	FName Name;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	FText Description;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	float Durability;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	float MaxDurability;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	float DurabilityPerUse;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	UTexture2D* Icon;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	UStaticMesh* Mesh;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	UStaticMesh* DropMesh;
};

USTRUCT(BlueprintType)
struct FItemGroup
{
	GENERATED_BODY()

public:
	FItemGroup() 
	{
		ItemId = "";
		Amount = Itens.Num();
		Icon = nullptr;
	}

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FString ItemId;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TArray<FItem> Itens;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	UTexture2D* Icon;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	int32 Amount;

	void AddItem(FItem NewItem)
	{
		if (Itens.Num() == 0)
		{
			ItemId = NewItem.Id;
			Icon = NewItem.Icon;
		}

		Itens.Add(NewItem);
		Amount = Itens.Num();
	}
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE (FOnInventoryChangedSignature);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class ZEND_API UMyInventoryComponent : public UActorComponent
{
	GENERATED_BODY()

public: 
	UPROPERTY(BlueprintAssignable)
	FOnInventoryChangedSignature OnInventoryChangedSignature;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "Item")
	UDataTable* ItemDataTable;

private:
	TArray<FItemGroup> ItemGroups;

public:	
	UMyInventoryComponent();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UFUNCTION(BlueprintCallable)
	void AddItem(FItem NewItem);
	UFUNCTION(BlueprintCallable)
	void AddItems(const TArray<FItem>& ItemsToAdd);

	UFUNCTION(BlueprintCallable)
	void AddItemByName(FName ItemName);

	UFUNCTION(BlueprintCallable, BlueprintPure)
	TArray<FItemGroup> GetItemGroups() const;
};
