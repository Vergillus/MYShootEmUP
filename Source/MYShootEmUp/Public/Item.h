#pragma once

#include "CoreMinimal.h"
#include "Item.generated.h"

UENUM(BlueprintType)
enum class EWeaponType : uint8
{
	EWT_Pistol UMETA(DisplayName = "Pistol"),
	EWT_SMG UMETA(DisplayName = "SMG"),
	EWT_Sniper UMETA(DisplayName = "Sniper"),

	EWT_MAX UMETA(DisplayName = "Default")
};
