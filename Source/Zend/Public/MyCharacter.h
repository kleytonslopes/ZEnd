// Copyright Sunydark 2022.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "MyCharacter.generated.h"

class UCameraComponent;
UCLASS()
class ZEND_API AMyCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly, Category="Camera")
	UCameraComponent* MyCamera;

public:
	AMyCharacter();

protected:
	virtual void BeginPlay() override;

public:	
	///virtual void Tick(float DeltaTime) override;

	UFUNCTION(BlueprintCallable, Category = "My Character")
	void LookUp(float value);

	UFUNCTION(BlueprintCallable, Category = "My Character")
	void Turn(float value);

	UFUNCTION(BlueprintCallable, Category= "My Character")
	void MoveForward(float value);

	UFUNCTION(BlueprintCallable, Category = "My Character")
	void MoveRight(float value);
};
