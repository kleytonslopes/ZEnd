// Copyright Sunydark 2022.


#include "MyContainer.h"
#include "Components/StaticMeshComponent.h"
#include "Components/MyInventoryComponent.h"
#include "DrawDebugHelpers.h"
#include "Components/MyLootBoxComponent.h"
#include "MyCharacter.h"

AMyContainer::AMyContainer()
{
	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	SetRootComponent(Mesh);

	Inventory = CreateDefaultSubobject<UMyInventoryComponent>(TEXT("Inventory"));
	LootBox = CreateDefaultSubobject<UMyLootBoxComponent>(TEXT("LootBox"));
	
}

void AMyContainer::BeginPlay()
{
	Super::BeginPlay();
	SetDefaults();
}

void AMyContainer::OnInteract(AActor* Caller)
{
//	FVector EndLocation = GetActorLocation();
//	EndLocation.Z += 150.f;
//
//	FHitResult HitResult;
//	bool Success;

//	TraceLine(ECollisionChannel::ECC_Visibility, EndLocation, HitResult, Success, true);

	if (bIsFirstOpen) 
	{
		LootBox->GenerateItems();
		Inventory->AddItems(LootBox->GetItems());
		bIsFirstOpen = false;
	}

	AMyCharacter* MyCharacter = Cast<AMyCharacter>(Caller);
	if (MyCharacter)
	{
		MyCharacter->SetContainerInventory(Inventory);
	}
}

bool AMyContainer::CanPickUp() const
{
	return false;
}

void AMyContainer::TraceLine(ECollisionChannel CollisionChannel, FVector EndLocation, FHitResult& HitResult, bool& Success, bool DrawDebug)
{
	FVector StartLocation = GetActorLocation();
	FCollisionQueryParams TraceParameters;
	TraceParameters.AddIgnoredActor(this);
	TraceParameters.bTraceComplex = true;


	Success = GetWorld()->LineTraceSingleByChannel(HitResult, StartLocation, EndLocation, CollisionChannel, TraceParameters);

	if (DrawDebug)
		DrawDebugLine(GetWorld(), StartLocation, EndLocation, FColor::Red, false, 2.0f);
}

void AMyContainer::SetDefaults()
{
	LootBox->LootBoxName = LootBoxName;
}

void AMyContainer::Interact_Implementation(AActor* Caller)
{
	OnInteract(Caller);
}
