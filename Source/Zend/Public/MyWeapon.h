// Copyright Sunydark 2022.

#pragma once

#include "CoreMinimal.h"
#include "MyItem.h"
#include "MyWeapon.generated.h"

/**
 * 
 */
UCLASS()
class ZEND_API AMyWeapon : public AMyItem
{
	GENERATED_BODY()
	
public:
	AMyWeapon();

	virtual void UseItem() override;
};
