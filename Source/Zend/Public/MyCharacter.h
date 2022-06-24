// Copyright Sunydark 2022.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "../Zend.h"
#include "Components/MyInventoryComponent.h"
#include "MyCharacter.generated.h"

class UCameraComponent;
class USpringArmComponent;
class UMyStatusManagerComponent;
class UMyStatusComponent;
class UMyInventoryComponent;
class AMyItem;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnIsWalkingSignature, bool, IsWalking);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnIsRunningSignature, bool, IsRunning);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnItemEquippedSignature, FItem, ItemEquipped);

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnHealthIsEmptySignature);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnMyHealthChangedSignature, float, Percent);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnMyHealthInDangerZoneSignature, bool, IsInDangerZone);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnMyThirstChangedSignature, float, Percent);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnMyThirstInDangerZoneSignature, bool, IsInDangerZone);

UCLASS()
class ZEND_API AMyCharacter : public ACharacter
{
	GENERATED_BODY()

private:
	AMyItem* EquippedItem;

public:
	UPROPERTY(BlueprintAssignable, Category = "My Character")
	FOnIsWalkingSignature OnIsWalkingSignature;
	UPROPERTY(BlueprintAssignable, Category = "My Character")
	FOnIsRunningSignature OnIsRunningSignature;
	UPROPERTY(BlueprintAssignable, Category = "My Character")
	FOnItemEquippedSignature OnItemEquippedSignature;

	UPROPERTY(BlueprintAssignable, Category = "My Character")
	FOnHealthIsEmptySignature OnHealthIsEmptySignature;
	UPROPERTY(BlueprintAssignable, Category = "My Character")
	FOnMyHealthChangedSignature OnMyHealthChangedSignature;
	UPROPERTY(BlueprintAssignable, Category = "My Character")
	FOnMyHealthInDangerZoneSignature OnMyHealthInDangerZoneSignature;

	UPROPERTY(BlueprintAssignable, Category = "My Character")
	FOnMyThirstChangedSignature OnMyThirstChangedSignature;
	UPROPERTY(BlueprintAssignable, Category = "My Character")
	FOnMyThirstInDangerZoneSignature OnMyThirstInDangerZoneSignature;

protected:
	UPROPERTY(EditDefaultsOnly, Category = "My Character|Components")
	USpringArmComponent* CameraOffsetArm;

	UPROPERTY(EditDefaultsOnly, Category = "My Character|Components")
	UCameraComponent* MyCamera;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "My Character|Components")
	UMyStatusManagerComponent* StatusManager;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "My Character|Components")
	UMyStatusComponent* HealthComponent;
	
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "My Character|Components")
	UMyStatusComponent* ThirstComponent;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "My Character|Components")
	UMyInventoryComponent* DefaultInventory;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "My Character|Inventory")
	TArray<FString> ItemsDefault;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "My Character")
	float MaxWalkSpeed = 125.f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "My Character")
	float MaxRunningSpeed = 600.f;

	UPROPERTY(BlueprintReadOnly, Category = "My Character")
	bool bWantToRun = false;

	UPROPERTY(BlueprintReadOnly, Category = "My Character")
	float CurrentSpeed;

	UPROPERTY(BlueprintReadOnly, Category = "My Character")
	bool bIsDead = false;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "My Character")
	float MaxZoomOut = 400.f;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "My Character")
	float MaxZoomIn = 10.f;

public:
	AMyCharacter();

protected:
	virtual void BeginPlay() override;

	void InterpolateWalkingSpeed(float DeltaTime);

public:	
	virtual void Tick(float DeltaTime) override;
	UFUNCTION()
	void OnTakeAnyDamageEvent(AActor* DamagedActor, float Damage, const class UDamageType* DamageType, class AController* InstigatedBy, AActor* DamageCauser);

	UFUNCTION(BlueprintCallable, Category = "My Character")
	void LookUp(float value);

	UFUNCTION(BlueprintCallable, Category = "My Character")
	void Turn(float value);

	UFUNCTION(BlueprintCallable, Category = "My Character")
	void MoveForward(float value);

	UFUNCTION(BlueprintCallable, Category = "My Character")
	void MoveRight(float value);

	UFUNCTION(BlueprintCallable, Category = "My Character")
	void ZoonIn(float value);
	UFUNCTION(BlueprintCallable, Category = "My Character")
	void ZoomOut(float value);

	UFUNCTION(BlueprintCallable, Category = "My Character")
	void BeginJump();

	UFUNCTION(BlueprintCallable, Category = "My Character")
	void BeginCrouch();

	UFUNCTION(BlueprintCallable, Category = "My Character")
	void EndCrouch();

	UFUNCTION(BlueprintCallable, Category = "My Character")
	void ToggleCrouch();

	UFUNCTION(BlueprintCallable, Category = "My Character")
	void BeginRun();

	UFUNCTION(BlueprintCallable, Category = "My Character")
	void EndRun();

	UFUNCTION(BlueprintCallable, Category = "My Character")
	void Interact();

	UFUNCTION(BlueprintCallable, Category = "My Character")
	void UsePrimaryItem();
public:
	UFUNCTION(Category = "My Character")
	void OnHealthIsEmptyEvent();
	UFUNCTION(Category = "My Character")
	void OnMyHealthChangedEvent(float Percent);
	UFUNCTION(Category = "My Character")
	void OnMyHealthInDangerZoneEvent(bool IsInDangerZone);

	UFUNCTION(Category = "My Character")
	void OnMyThirstChangedEvent(float Percent);
	UFUNCTION(Category = "My Character")
	void OnMyThirstInDangerZoneEvent(bool IsInDangerZone);

	UFUNCTION(Category = "My Character")
	void OnDefaultInventoryChangedEvent();

	UFUNCTION(BlueprintImplementableEvent, Category = "My Character")
	void CreateDefaultInventory();

	UFUNCTION(BlueprintCallable, Category = "My Character")
	void AddItemsToInventory(const TArray<FItem>& ItemsToAdd);

	UFUNCTION(BlueprintCallable, Category = "My Character")
	void SetContainerInventory(UMyInventoryComponent* ContainerInventory);

	UFUNCTION(BlueprintCallable, Category = "My Character")
	void GetAllItemsFromInventory(UMyInventoryComponent* OtherInventory);

	UFUNCTION(BlueprintCallable, Category = "My Character")
	void SetItemEquipped(AMyItem* ItemToEquipped);

	UFUNCTION()
	void OnPrimaryItemUsedEvent();

	UMyStatusComponent* GetHealthComponent() const;
	UMyStatusComponent* GetThirstComponent() const;
	TArray<FItemDataSaveGame> GetItemsToSaveData() const;
private:
	void ConfigureStatusComponents();
	void ConfigureDefaultInventoryComponent();
	bool CheckIfStatusInDangerZone() const;
	void TraceEyesLine(ECollisionChannel CollisionChannel, FVector EndLocation, FHitResult& HitResult, bool& Success, bool DrawDebug);
	void KillSelf();
};
