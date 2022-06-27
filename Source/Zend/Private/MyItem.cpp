// Copyright Sunydark 2022.


#include "MyItem.h"
#include "Engine/DataTable.h"
#include "UObject/ConstructorHelpers.h"
#include "../Zend.h"
#include "MyCharacter.h"

AMyItem::AMyItem()
{
	static ConstructorHelpers::FObjectFinder<UDataTable> ItemDataTableObject(TEXT(DATA_ITEM));
	if (ItemDataTableObject.Succeeded()) ItemDataTable = ItemDataTableObject.Object;

	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));

	Mesh->SetCollisionObjectType(COLLISION_ITEM_CHANNEL);

	SetRootComponent(Mesh);
}

void AMyItem::BeginPlay()
{
	Super::BeginPlay();
	
}

void AMyItem::UseItem()
{
	
	if (ItemInfor.Durability > 0.f)
		ItemInfor.Durability = FMath::Clamp((ItemInfor.Durability - ItemInfor.DurabilityPerUse), 0.f, ItemInfor.MaxDurability);

	ApplyStateToOwner();

	if (ItemInfor.Durability <= 0.f)
		Destroy();

	OnItemUsedSignature.Broadcast();
}

void AMyItem::ApplyStateToOwner()
{
	if (GetOwner())
	{
		AMyCharacter* MyCharacter = Cast<AMyCharacter>(GetOwner());

		if (MyCharacter)
		{
			MyCharacter->ApplyItemStatus(ItemInfor);
		}
	}
}
