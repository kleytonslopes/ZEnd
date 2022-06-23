// Copyright Sunydark 2022.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Engine/DataTable.h"
#include "MyInventoryComponent.h"
#include "MyLootBoxComponent.generated.h"

USTRUCT(BlueprintType)
struct FItemLootBox
{
	GENERATED_BODY()
public:
	FItemLootBox()
	{
		ItemName = FName("");
		MinAmount = 1;
		MaxAmount = 1;
		Chance = 0.5f;
		ChancePattern = 1.0f;
		bCanGenerate = false;
	}

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	FName ItemName;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	int32 MinAmount;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	int32 MaxAmount;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	float Chance;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	float ChancePattern;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite)
	bool bCanGenerate;
	
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite)
	int32 AmountToGenerate;

	void VerifyCanGenerate()
	{
		float randFloat = FMath::FRandRange(0.f, ChancePattern);

		if(Chance > randFloat)
			bCanGenerate = true;
		else
			bCanGenerate = false;
	}

	void CalculeAmountToGenerate()
	{
		int32 intRand = FMath::RandRange(MinAmount, MaxAmount);

		AmountToGenerate = intRand;
	}
};

USTRUCT(BlueprintType)
struct FLootBox : public FTableRowBase
{
	GENERATED_BODY()

public:
	FLootBox()
	{
		Id = "";
		Name = FName("");
	}

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	FString Id;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	FName Name;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	TArray<FItemLootBox> Items;

};

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class ZEND_API UMyLootBoxComponent : public UActorComponent
{
	GENERATED_BODY()

private:
	TArray<FItem> Items;

public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "LootBox")
	FName LootBoxName;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "Item")
	UDataTable* LootBoxDataTable;
	
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "Item")
	UDataTable* ItemDataTable;
	

public:	
	UMyLootBoxComponent();

protected:
	virtual void BeginPlay() override;

public:	
	UFUNCTION(BlueprintCallable)
	void GenerateItems();

	UFUNCTION(BlueprintCallable, BlueprintPure)
	TArray<FItem> GetItems() const;
private:
	FItem FindItem(FName ItemName) const;
		
};
