// Copyright Sunydark 2022.


#include "MyCharacter.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "Components/MyStatusManagerComponent.h"
#include "Components/MyStatusComponent.h"
#include "Components/MyInventoryComponent.h"
#include "DrawDebugHelpers.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Interfaces/MyInteractableInterface.h"
#include "MyPlayerController.h"
#include "Utils/MySaveGame.h"
#include "MyItem.h"
#include "../Zend.h"

AMyCharacter::AMyCharacter()
{
	///PrimaryActorTick.bCanEverTick = true;

	GetCapsuleComponent()->SetCollisionResponseToChannel(COLLISION_ITEM_CHANNEL, ECR_Ignore);

	GetMesh()->SetCollisionResponseToChannel(COLLISION_ITEM_CHANNEL, ECR_Ignore);

	GetMovementComponent()->NavAgentProps.bCanCrouch = true;
	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->bCanWalkOffLedgesWhenCrouching = true;

	GetCapsuleComponent()->bHiddenInGame = true;

	bUseControllerRotationPitch = false;
	bUseControllerRotationRoll = false;
	bUseControllerRotationYaw = false;

	CameraOffsetArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraOffsetArm"));
	CameraOffsetArm->SetupAttachment(GetCapsuleComponent());
	CameraOffsetArm->TargetArmLength = 400.f;
	CameraOffsetArm->bUsePawnControlRotation = true;
	CameraOffsetArm->SetRelativeLocation(FVector(0.f, 0.f, 50.f));

	MyCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("MyCamera"));
	MyCamera->SetupAttachment(CameraOffsetArm);
	MyCamera->bUsePawnControlRotation = false;

	StatusManager = CreateDefaultSubobject<UMyStatusManagerComponent>(TEXT("StatusManager"));
	HealthComponent = CreateDefaultSubobject<UMyStatusComponent>(TEXT("HealthComponent"));
	ThirstComponent = CreateDefaultSubobject<UMyStatusComponent>(TEXT("ThirstComponent"));

	DefaultInventory = CreateDefaultSubobject<UMyInventoryComponent>(TEXT("DefaultInventory"));

	
}

void AMyCharacter::BeginPlay()
{
	Super::BeginPlay();

	InitializeDefaults();

	ConfigureStatusComponents();

	ConfigureDefaultInventoryComponent();
}

void AMyCharacter::InterpolateWalkingSpeed(float DeltaTime)
{
	float newSpeed = CurrentSpeed;
	if (bWantToRun && GetCharacterMovement()->MaxWalkSpeed < MaxRunningSpeed)
	{
		newSpeed = FMath::FInterpTo(CurrentSpeed, MaxRunningSpeed, DeltaTime, 1.0f);
	}
	CurrentSpeed = newSpeed;
	GetCharacterMovement()->MaxWalkSpeed = newSpeed;
	UE_LOG(LogTemp, Warning, TEXT("Speed: %f"), GetCharacterMovement()->MaxWalkSpeed);
}

UMyStatusComponent* AMyCharacter::GetHealthComponent() const
{
	return HealthComponent;
}

UMyStatusComponent* AMyCharacter::GetThirstComponent() const
{
	return ThirstComponent;
}

TArray<FItemDataSaveGame> AMyCharacter::GetItemsToSaveData() const
{
	return DefaultInventory->GetItemsToSaveGame();
}

void AMyCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AMyCharacter::OnTakeAnyDamageEvent(AActor* DamagedActor, float Damage, const class UDamageType* DamageType, class AController* InstigatedBy, AActor* DamageCauser)
{
	HealthComponent->DecreaseValue(Damage);
}

void AMyCharacter::LookUp(float value)
{
	if (bIsDead) return;
	if (Controller && value != 0.f)
	{
		AddControllerPitchInput(-value);
	}
}

void AMyCharacter::Turn(float value)
{
	if (bIsDead) return;
	if (Controller && value != 0.f)
	{
		AddControllerYawInput(value);
	}
}

void AMyCharacter::MoveForward(float value)
{
	if (bIsDead) return;
	if (Controller && value != 0.f)
	{
		const FRotator rotation = Controller->GetControlRotation();
		const FRotator yawRotation(0, rotation.Yaw, 0);
		const FVector direction = FRotationMatrix(yawRotation).GetUnitAxis(EAxis::X);

		AddMovementInput(direction, value);

		OnIsWalkingSignature.Broadcast(true);
		OnIsRunningSignature.Broadcast(bWantToRun);

		if (CheckIfStatusInDangerZone())
			EndRun();
	}

	if (GetVelocity().Length() <= 0.f)
	{
		OnIsWalkingSignature.Broadcast(false);
		OnIsRunningSignature.Broadcast(false);
	}
}

void AMyCharacter::MoveRight(float value)
{
	if (bIsDead) return;
	if (Controller && value != 0.f)
	{
		const FRotator rotation = Controller->GetControlRotation();
		const FRotator yawRotation(0, rotation.Yaw, 0);
		const FVector direction = FRotationMatrix(yawRotation).GetUnitAxis(EAxis::Y);

		AddMovementInput(direction, value);

		OnIsWalkingSignature.Broadcast(true);
		OnIsRunningSignature.Broadcast(bWantToRun);

		if (CheckIfStatusInDangerZone())
			EndRun();
	}
	if (GetVelocity().Length() <= 0.f)
	{
		OnIsWalkingSignature.Broadcast(false);
		OnIsRunningSignature.Broadcast(false);
	}
}

void AMyCharacter::ZoonIn(float value)
{
	float currentZoom = CameraOffsetArm->TargetArmLength;
	float newZoom = FMath::Clamp(currentZoom + 10, MaxZoomIn, MaxZoomOut);
	CameraOffsetArm->TargetArmLength = newZoom;
}

void AMyCharacter::ZoomOut(float value)
{
	float currentZoom = CameraOffsetArm->TargetArmLength;
	float newZoom = FMath::Clamp(currentZoom - 10, MaxZoomIn, MaxZoomOut);

	CameraOffsetArm->TargetArmLength = newZoom;
}

void AMyCharacter::BeginJump()
{
	if (bIsDead) return;
	if (Controller)
	{
		if (!CheckIfStatusInDangerZone())
			Jump();
	}
}

void AMyCharacter::BeginCrouch()
{
	if (bIsDead) return;
	if (Controller)
	{
		Crouch();
	}
}

void AMyCharacter::EndCrouch()
{
	if (bIsDead) return;
	if (Controller)
	{
		UnCrouch();
	}
}

void AMyCharacter::ToggleCrouch()
{
	if (GetMovementComponent()->IsCrouching())
		EndCrouch();
	else
		BeginCrouch();
}

void AMyCharacter::BeginRun()
{
	if (bIsDead) return;
	if (Controller)
	{
		if (CheckIfStatusInDangerZone())
			return;

		bWantToRun = true;
		GetCharacterMovement()->MaxWalkSpeed = MaxRunningSpeed;
	}
}

void AMyCharacter::EndRun()
{
	if (Controller)
	{
		bWantToRun = false;
		GetCharacterMovement()->MaxWalkSpeed = MaxWalkSpeed;
		if (CheckIfStatusInDangerZone())
			GetCharacterMovement()->MaxWalkSpeed = MaxWalkSpeed / 2;
	}
}

void AMyCharacter::Interact()
{
	if (bIsDead) return;

	FHitResult MouseHitResult;
	APlayerController* PC = Cast<APlayerController>(GetController());
	bool bMouseHitResult = PC->GetHitResultUnderCursor(ECollisionChannel::ECC_Visibility, true, MouseHitResult);
	if (bMouseHitResult)
	{
		FVector StartLocation;
		FRotator Rotation;
		FVector EndLocation;

		FHitResult HitResult;
		bool Success;


		GetActorEyesViewPoint(StartLocation, Rotation);
		FVector MouseLocation = MouseHitResult.ImpactPoint;
		FVector FakeEnd = FVector(MouseLocation - StartLocation);

		EndLocation = StartLocation + (FakeEnd.Rotation().Vector() * 400);

		///UUserWidget* InteractionTex = InteractionTextWidget->GetWidget();

		TraceEyesLine(ECollisionChannel::ECC_Visibility, EndLocation, HitResult, Success, true);

		AActor* HitActor = HitResult.GetActor();

		if (HitActor)
		{
			///if (InteractionTex)
			///{
			bool bHasInterface = UKismetSystemLibrary::DoesImplementInterface(HitActor, UMyInteractableInterface::StaticClass());
			if (bHasInterface)
			{
				IMyInteractableInterface::Execute_Interact(HitActor, this);
				/*AMyPlayerController* MyPC = Cast<AMyPlayerController>(GetController());
				if (MyPC)
				{
					AMyInteractables* Interacted = Cast<AMyInteractables>(HitActor);
					FVector InteractedLocation = HitActor->GetActorLocation() + Interacted->PlayerPointToInteract;
					MyPC->MoveToTargetLocation(InteractedLocation);
				}*/
			}
			///}
		}
	}
}

void AMyCharacter::UsePrimaryItem()
{
	if (!EquippedItem || bIsDead)
		return;

	EquippedItem->UseItem();

	if (EquippedItem->ItemInfor.Durability <= 0.f)
	{

		EquippedItem->Destroy();
		OnItemEquippedSignature.Broadcast(FItem());
	}
}

void AMyCharacter::OnHealthIsEmptyEvent()
{
	KillSelf();
}

void AMyCharacter::OnMyHealthChangedEvent(float Percent)
{
	OnMyHealthChangedSignature.Broadcast(Percent);
}

void AMyCharacter::OnMyHealthInDangerZoneEvent(bool IsInDangerZone)
{
	OnMyHealthInDangerZoneSignature.Broadcast(IsInDangerZone);
}

void AMyCharacter::OnMyThirstChangedEvent(float Percent)
{
	OnMyThirstChangedSignature.Broadcast(Percent);
}

void AMyCharacter::OnMyThirstInDangerZoneEvent(bool IsInDangerZone)
{
	OnMyThirstInDangerZoneSignature.Broadcast(IsInDangerZone);
}

void AMyCharacter::OnDefaultInventoryChangedEvent()
{
	AMyPlayerController* MyPlayerController = Cast<AMyPlayerController>(Controller);
	if (MyPlayerController)
	{
		MyPlayerController->CallUpdatePlayerDefaultInventoryUI(DefaultInventory->GetItemGroups());
	}
}

void AMyCharacter::AddItemsToInventory(const TArray<FItem>& ItemsToAdd)
{
	if (DefaultInventory)
	{
		DefaultInventory->AddItems(ItemsToAdd);
	}
}

void AMyCharacter::SetContainerInventory(UMyInventoryComponent* ContainerInventory)
{
	AMyPlayerController* MyPlayerController = Cast<AMyPlayerController>(Controller);
	if (MyPlayerController)
	{
		MyPlayerController->SetContainerInventory(ContainerInventory);
	}
}

void AMyCharacter::GetAllItemsFromInventory(UMyInventoryComponent* OtherInventory)
{
	AMyPlayerController* MyPlayerController = Cast<AMyPlayerController>(Controller);
	if (OtherInventory && MyPlayerController)
	{
		if (MyPlayerController)
		{
			MyPlayerController->TransferAllItems(OtherInventory, DefaultInventory);
		}
	}
}

void AMyCharacter::SetItemEquipped(AMyItem* ItemToEquipped)
{
	if (!ItemToEquipped)
		return;

	if (EquippedItem)
		EquippedItem->Destroy();

	EquippedItem = ItemToEquipped;
	ItemToEquipped->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetIncludingScale, FName("PrimaryItemSlot"));

	ItemToEquipped->OnItemUsedSignature.Clear();
	ItemToEquipped->OnItemUsedSignature.AddDynamic(this, &AMyCharacter::OnPrimaryItemUsedEvent);

	OnItemEquippedSignature.Broadcast(ItemToEquipped->ItemInfor);
}

void AMyCharacter::OnPrimaryItemUsedEvent()
{
	if (EquippedItem)
		OnItemEquippedSignature.Broadcast(EquippedItem->ItemInfor);
	else
		OnItemEquippedSignature.Broadcast(FItem());
}

void AMyCharacter::InitializeDefaults()
{
	CurrentSpeed = MaxWalkSpeed;
	GetCharacterMovement()->MaxWalkSpeed = MaxWalkSpeed;
	GetCharacterMovement()->MaxWalkSpeedCrouched = MaxWalkSpeed / 2;
}

void AMyCharacter::ConfigureStatusComponents()
{
	OnTakeAnyDamage.AddDynamic(this, &AMyCharacter::OnTakeAnyDamageEvent);

	HealthComponent->OnStatusValueChangedSignature.AddDynamic(this, &AMyCharacter::OnMyHealthChangedEvent);
	HealthComponent->OnStatusValueInDangerZoneSignature.AddDynamic(this, &AMyCharacter::OnMyHealthInDangerZoneEvent);
	HealthComponent->OnAlertIfOnLimitSignature.AddDynamic(this, &AMyCharacter::OnHealthIsEmptyEvent);
	OnMyHealthChangedEvent(HealthComponent->GetPercent());
	StatusManager->SetHealthComponent(HealthComponent);

	ThirstComponent->OnStatusValueChangedSignature.AddDynamic(this, &AMyCharacter::OnMyThirstChangedEvent);
	ThirstComponent->OnStatusValueInDangerZoneSignature.AddDynamic(this, &AMyCharacter::OnMyThirstInDangerZoneEvent);
	OnMyThirstChangedEvent(ThirstComponent->GetPercent());
	StatusManager->SetThirstComponent(ThirstComponent);
}

void AMyCharacter::ConfigureDefaultInventoryComponent()
{
	DefaultInventory->OnInventoryChangedSignature.AddDynamic(this, &AMyCharacter::OnDefaultInventoryChangedEvent);

	CreateDefaultInventory();
}

bool AMyCharacter::CheckIfStatusInDangerZone() const
{
	return HealthComponent->GetIfInDangerZone();
}

void AMyCharacter::TraceEyesLine(ECollisionChannel CollisionChannel, FVector EndLocation, FHitResult& HitResult, bool& Success, bool DrawDebug)
{
	FVector StartLocation;
	FRotator Rotation;
	FCollisionQueryParams TraceParameters;
	TraceParameters.AddIgnoredActor(GetOwner());
	TraceParameters.AddIgnoredActor(this);
	TraceParameters.bTraceComplex = true;

	GetActorEyesViewPoint(StartLocation, Rotation);

	Success = GetWorld()->LineTraceSingleByChannel(HitResult, StartLocation, EndLocation, ECC_Pawn, TraceParameters);

	if (DrawDebug)
		DrawDebugLine(GetWorld(), StartLocation, EndLocation, FColor::Red, false, 2.0f);
}

void AMyCharacter::KillSelf()
{
	if (!bIsDead)
	{
		bIsDead = true;

		Destroy();
	}
}

