// Copyright Sunydark 2022.


#include "MyCharacter.h"
#include "Components/CapsuleComponent.h"

AMyCharacter::AMyCharacter()
{
	///PrimaryActorTick.bCanEverTick = true;

	GetCapsuleComponent()->bHiddenInGame = false;


}

void AMyCharacter::BeginPlay()
{
	Super::BeginPlay();
	
}

///void AMyCharacter::Tick(float DeltaTime)
///{
///	Super::Tick(DeltaTime);
///
///}

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
	}
}