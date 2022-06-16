// Copyright Sunydark 2022.


#include "MyCharacter.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "Components/MyStatusManagerComponent.h"
#include "Components/MyStatusComponent.h"

AMyCharacter::AMyCharacter()
{
	///PrimaryActorTick.bCanEverTick = true;

	GetMovementComponent()->NavAgentProps.bCanCrouch = true;
	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->bCanWalkOffLedgesWhenCrouching = true;
	GetCharacterMovement()->MaxWalkSpeed = MaxWalkSpeed;

	GetCapsuleComponent()->bHiddenInGame = false;

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

	CurrentSpeed = MaxWalkSpeed;
}

void AMyCharacter::BeginPlay()
{
	Super::BeginPlay();
	
	ConfigureStatusComponents();
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

void AMyCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AMyCharacter::LookUp(float value)
{
	if (Controller && value != 0.f)
	{
		AddControllerPitchInput(-value);
	}
}

void AMyCharacter::Turn(float value)
{
	if (Controller && value != 0.f)
	{
		AddControllerYawInput(value);
	}
}

void AMyCharacter::MoveForward(float value)
{
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

void AMyCharacter::BeginJump()
{
	if (Controller)
	{
		if (!CheckIfStatusInDangerZone())
			Jump();
	}
}

void AMyCharacter::BeginCrouch()
{
	if (Controller)
	{
		Crouch();
	}
}

void AMyCharacter::EndCrouch()
{
	if (Controller)
	{
		UnCrouch();
	}
}

void AMyCharacter::ToggleCrouch()
{
	if(GetMovementComponent()->IsCrouching())
		EndCrouch();
	else
		BeginCrouch();
}

void AMyCharacter::BeginRun()
{
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
		if(CheckIfStatusInDangerZone())
			GetCharacterMovement()->MaxWalkSpeed = MaxWalkSpeed / 2;
	}
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

void AMyCharacter::ConfigureStatusComponents()
{
	HealthComponent->OnStatusValueChangedSignature.AddDynamic(this, &AMyCharacter::OnMyHealthChangedEvent);
	HealthComponent->OnStatusValueInDangerZoneSignature.AddDynamic(this, &AMyCharacter::OnMyHealthInDangerZoneEvent);
	OnMyHealthChangedEvent(HealthComponent->GetPercent());
	StatusManager->SetHealthComponent(HealthComponent);

	ThirstComponent->OnStatusValueChangedSignature.AddDynamic(this, &AMyCharacter::OnMyThirstChangedEvent);
	ThirstComponent->OnStatusValueInDangerZoneSignature.AddDynamic(this, &AMyCharacter::OnMyThirstInDangerZoneEvent);
	OnMyThirstChangedEvent(ThirstComponent->GetPercent());
	StatusManager->SetThirstComponent(ThirstComponent);
}

bool AMyCharacter::CheckIfStatusInDangerZone() const
{
	return HealthComponent->GetIfInDangerZone();	
}

