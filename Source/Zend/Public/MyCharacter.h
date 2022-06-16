// Copyright Sunydark 2022.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "../Zend.h"
#include "MyCharacter.generated.h"



class UCameraComponent;
class USpringArmComponent;
class UMyStatusManagerComponent;
class UMyStatusComponent;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnIsWalkingSignature, bool, IsWalking);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnIsRunningSignature, bool, IsRunning);

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnMyHealthChangedSignature, float, Percent);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnMyHealthInDangerZoneSignature, bool, IsInDangerZone);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnMyThirstChangedSignature, float, Percent);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnMyThirstInDangerZoneSignature, bool, IsInDangerZone);

UCLASS()
class ZEND_API AMyCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintAssignable, Category = CATEGORY_MY_CHARACTER)
	FOnIsWalkingSignature OnIsWalkingSignature;
	UPROPERTY(BlueprintAssignable, Category = CATEGORY_MY_CHARACTER)
	FOnIsRunningSignature OnIsRunningSignature;

	UPROPERTY(BlueprintAssignable, Category = CATEGORY_MY_CHARACTER)
	FOnMyHealthChangedSignature OnMyHealthChangedSignature;
	UPROPERTY(BlueprintAssignable, Category = CATEGORY_MY_CHARACTER)
	FOnMyHealthInDangerZoneSignature OnMyHealthInDangerZoneSignature;

	UPROPERTY(BlueprintAssignable, Category = CATEGORY_MY_CHARACTER)
	FOnMyThirstChangedSignature OnMyThirstChangedSignature;
	UPROPERTY(BlueprintAssignable, Category = CATEGORY_MY_CHARACTER)
	FOnMyThirstInDangerZoneSignature OnMyThirstInDangerZoneSignature;

protected:
	UPROPERTY(EditDefaultsOnly, Category = CATEGORY_CAMERA)
	USpringArmComponent* CameraOffsetArm;

	UPROPERTY(EditDefaultsOnly, Category = CATEGORY_CAMERA)
	UCameraComponent* MyCamera;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = CATEGORY_MY_CHARACTER)
	UMyStatusManagerComponent* StatusManager;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly)
	UMyStatusComponent* HealthComponent;
	
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly)
	UMyStatusComponent* ThirstComponent;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = CATEGORY_MY_CHARACTER)
	float MaxWalkSpeed = 200.f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = CATEGORY_MY_CHARACTER)
	float MaxRunningSpeed = 600.f;

	UPROPERTY(BlueprintReadOnly, Category = CATEGORY_MY_CHARACTER)
	bool bWantToRun = false;

	UPROPERTY(BlueprintReadOnly, Category = CATEGORY_MY_CHARACTER)
	float CurrentSpeed;

public:
	AMyCharacter();

protected:
	virtual void BeginPlay() override;

	void InterpolateWalkingSpeed(float DeltaTime);

public:	
	virtual void Tick(float DeltaTime) override;

	UFUNCTION(BlueprintCallable, Category = CATEGORY_MY_CHARACTER)
	void LookUp(float value);

	UFUNCTION(BlueprintCallable, Category = CATEGORY_MY_CHARACTER)
	void Turn(float value);

	UFUNCTION(BlueprintCallable, Category = CATEGORY_MY_CHARACTER)
	void MoveForward(float value);

	UFUNCTION(BlueprintCallable, Category = CATEGORY_MY_CHARACTER)
	void MoveRight(float value);

	UFUNCTION(BlueprintCallable, Category = CATEGORY_MY_CHARACTER)
	void BeginJump();

	UFUNCTION(BlueprintCallable, Category = CATEGORY_MY_CHARACTER)
	void BeginCrouch();

	UFUNCTION(BlueprintCallable, Category = CATEGORY_MY_CHARACTER)
	void EndCrouch();

	UFUNCTION(BlueprintCallable, Category = CATEGORY_MY_CHARACTER)
	void ToggleCrouch();

	UFUNCTION(BlueprintCallable, Category = CATEGORY_MY_CHARACTER)
	void BeginRun();

	UFUNCTION(BlueprintCallable, Category = CATEGORY_MY_CHARACTER)
	void EndRun();

public:
	UFUNCTION(Category = CATEGORY_MY_CHARACTER)
	void OnMyHealthChangedEvent(float Percent);
	UFUNCTION(Category = CATEGORY_MY_CHARACTER)
	void OnMyHealthInDangerZoneEvent(bool IsInDangerZone);

	UFUNCTION(Category = CATEGORY_MY_CHARACTER)
	void OnMyThirstChangedEvent(float Percent);
	UFUNCTION(Category = CATEGORY_MY_CHARACTER)
	void OnMyThirstInDangerZoneEvent(bool IsInDangerZone);

private:
	void ConfigureStatusComponents();
	bool CheckIfStatusInDangerZone() const;
};
