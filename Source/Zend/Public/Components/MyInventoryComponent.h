// Copyright Sunydark 2022.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Engine/DataTable.h"
#include "MyInventoryComponent.generated.h"

class UTexture2D;
class AMyItem;

UENUM(BlueprintType)
enum EItemType
{
	IT_None UMETA(DisplayName = "None"),
	IT_Consumable UMETA(DisplayName = "Consumable"),
	IT_Weapon UMETA(DisplayName = "Weapon")
};

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
		ItemType = EItemType::IT_None;
		ChangeHealth = 0.f;
		ChangeThirst = 0.f;
		DurabilityPerUse = 1.f;
		ItemClass = nullptr;

		GenerateNewCode();
	}

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Defaults")
	FString Id;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Defaults")
	FString Code;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Defaults")
	FName Name;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Defaults")
	FText Description;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Defaults")
	float Durability;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Defaults")
	float MaxDurability;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Defaults")
	float DurabilityPerUse;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Defaults")
	TEnumAsByte<EItemType> ItemType;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Consumable")
	float ChangeHealth;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Consumable")
	float ChangeThirst;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Defaults")
	UTexture2D* Icon;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Defaults")
	UStaticMesh* Mesh;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Defaults")
	UStaticMesh* DropMesh;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Defaults")
	TSubclassOf<AMyItem> ItemClass;

	bool IsValid()
	{
		if(Id != "")
			return true;

		return false;
	}

	void GenerateNewCode()
	{
		Code = FGuid::NewGuid().ToString();
		UE_LOG(LogTemp, Warning, TEXT("New Code %s generated to Item %s"), *Code, *Id);
	}
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

USTRUCT(BlueprintType)
struct FItemDataSaveGame
{
	GENERATED_BODY();

public:
	FItemDataSaveGame()
	{
		ItemName = FName("");
		ItemCode = "";
		ItemDurability = 0;
	}

	UPROPERTY(BlueprintReadWrite)
		FName ItemName;

	UPROPERTY(BlueprintReadWrite)
		FString ItemCode;

	UPROPERTY(BlueprintReadWrite)
		float ItemDurability;

	void SetData(FItem ItemData)
	{
		ItemName = ItemData.Name;
		ItemCode = ItemData.Code;
		ItemDurability = ItemData.Durability;
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

	UFUNCTION(BlueprintCallable, BlueprintPure)
	TArray<FItem> GetItems() const;

	UFUNCTION(BlueprintCallable, BlueprintPure)
	FItem GetItemByCode(FString ItemCode, bool& HasItem, int32& GroupIndex, int32& ItemIndex);

	UFUNCTION(BlueprintCallable)
	void RemoveItemFromCode(FString ItemCode, bool& WasRemoved);

	UFUNCTION(BlueprintCallable)
	void RemoveItem(int32 GroupIndex, int32 ItemIndex);

	UFUNCTION(BlueprintCallable, BlueprintPure)
	TArray<FItemDataSaveGame> GetItemsToSaveGame() const;

};
