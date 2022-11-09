#pragma once

#include "CoreMinimal.h"
#include "Helper.generated.h"

UENUM(BlueprintType)
enum class EWeaponType : uint8
{
	EWT_Pistol UMETA(DisplayName = "Pistol"),
	EWT_SMG UMETA(DisplayName = "SMG"),
	EWT_Sniper UMETA(DisplayName = "Sniper"),

	EWT_MAX UMETA(DisplayName = "Default")
};


UENUM(BlueprintType)
enum class ESquadFormationType : uint8
{
	ESF_BackToBack UMETA(DisplayName = "BackToBack"),
	ESF_LineH UMETA(DisplayName = "ShoulderToShoulder"),
	ESF_LineV UMETA(DisplayName = "Line Vertical")
};