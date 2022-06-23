// Copyright Sunydark 2022.


#include "Components/MyLootBoxComponent.h"
#include "Components/MyInventoryComponent.h"
#include "Engine/DataTable.h"
#include "../Zend.h"

UMyLootBoxComponent::UMyLootBoxComponent()
{
	static ConstructorHelpers::FObjectFinder<UDataTable> LootBoxDataTableObject(TEXT(DATA_LOOTBOX));
	static ConstructorHelpers::FObjectFinder<UDataTable> ItemDataTableObject(TEXT(DATA_ITEM));

	if (LootBoxDataTableObject.Succeeded()) LootBoxDataTable = LootBoxDataTableObject.Object;
	if (ItemDataTableObject.Succeeded()) ItemDataTable = ItemDataTableObject.Object;
}

void UMyLootBoxComponent::BeginPlay()
{
	Super::BeginPlay();
}

void UMyLootBoxComponent::GenerateItems()
{
	if (LootBoxDataTable)
	{
		static const FString ContextString(TEXT("LootBox String Context"));
		const FLootBox* LootBoxFound = LootBoxDataTable->FindRow<FLootBox>(LootBoxName, ContextString, true);
		if (LootBoxFound)
		{
			for (int32 i = 0 ; i < LootBoxFound->Items.Num(); i++)
			{
				FItemLootBox ItemLoot = LootBoxFound->Items[i];

				ItemLoot.VerifyCanGenerate();
				if (ItemLoot.bCanGenerate)
				{
					FItem ItemToGenerate = FindItem(ItemLoot.ItemName);
					ItemLoot.CalculeAmountToGenerate();

					for (int32 a = 0; a < ItemLoot.AmountToGenerate; a++)
					{
						Items.Add(ItemToGenerate);
					}
				}
			}
		}
	}
}

TArray<FItem> UMyLootBoxComponent::GetItems() const
{
	return Items;
}

FItem UMyLootBoxComponent::FindItem(FName ItemName) const
{
	if (ItemDataTable)
	{
		static const FString ContextString(TEXT("Item String Context"));
		const FItem* ItemFound = ItemDataTable->FindRow<FItem>(ItemName, ContextString, true);

		return *(ItemFound);
	}

	return FItem();
}
