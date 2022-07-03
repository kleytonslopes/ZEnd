// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"

#define CATEGORY_CAMERA       "Camera"
#define CATEGORY_MY_CHARACTER "My Character"
#define CATEGORY_MY_STATUS    "My Status"

#define DATA_ITEM    "DataTable'/Game/Data/DT_Items.DT_Items'"
#define DATA_LOOTBOX "DataTable'/Game/Data/DT_LootBox.DT_LootBox'"

#define SETTING_GAMENAME         FName(TEXT("GAMENAME"))
#define SETTING_SERVERNAME       FName(TEXT("SERVERNAME"))
#define SETTING_NUMPLAYERSONLINE FName(TEXT("NUMPLAYERSONLINE"))
#define SETTING_MAXPLAYERS       FName(TEXT("MAXPLAYERS"))

#define COLLISION_ITEM_CHANNEL ECollisionChannel::ECC_GameTraceChannel1
#define INTERACT_TRACE_CHANNEL ECollisionChannel::ECC_GameTraceChannel2