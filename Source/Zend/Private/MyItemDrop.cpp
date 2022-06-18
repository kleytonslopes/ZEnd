// Copyright Sunydark 2022.


#include "MyItemDrop.h"
#include "Components/StaticMeshComponent.h"
#include "UObject/ConstructorHelpers.h"
#include "Engine/DataTable.h"
#include "../Zend.h"
#include "MyCharacter.h"

AMyItemDrop::AMyItemDrop()
{
	static ConstructorHelpers::FObjectFinder<UDataTable> ItemDataTableObject(TEXT(DATA_ITEM));
	if (ItemDataTableObject.Succeeded()) ItemDataTable = ItemDataTableObject.Object;

	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	SetRootComponent(Mesh);
}

void AMyItemDrop::BeginPlay()
{
	Super::BeginPlay();
	
}

void AMyItemDrop::OnInteract(AActor* Caller)
{
	if (Caller)
	{
		AMyCharacter* CharacterCaller = Cast<AMyCharacter>(Caller);
		if (CharacterCaller)
		{
			CharacterCaller->AddItemsToInventory(Items);
			Destroy();
		}
	}
}

void AMyItemDrop::AddItem(FItem ItemToAdd)
{
	if (Items.Num() > 0)
	{
		if(Items[0].Id == ItemToAdd.Id)
			Items.Add(ItemToAdd);
	}
	else
	{
		Items.Add(ItemToAdd);
		ItemId = ItemToAdd.Id;
		Mesh->SetStaticMesh(ItemToAdd.DropMesh);
	}

}

void AMyItemDrop::AddItemByName(FName ItemName)
{
	if (ItemDataTable)
	{
		static const FString ContextString(TEXT("Item String Context"));
		const FItem* ItemFound = ItemDataTable->FindRow<FItem>(ItemName, ContextString, true);

		if (ItemFound)
		{
			AddItem(*ItemFound);
		}
	}
}

void AMyItemDrop::CreateDefaults()
{
	if (DefaultItemsNames.Num())
	{
		int32 totalItens = DefaultItemsNames.Num();

		for (int32 i = 0; i < totalItens; i++)
		{
			AddItemByName(DefaultItemsNames[i]);
		}

	}
}

void AMyItemDrop::Interact_Implementation(AActor* Caller)
{
	OnInteract(Caller);
}
