#pragma once

#include "AmmunitionType.generated.h"

/**
 * Ammo types
 */
UENUM(BlueprintType)
enum class EAmmunition : uint8
{
	NONE		UMETA(DisplayName = "NONE"),
	SU_545		UMETA(DisplayName = "SU_545"),
	NATO_556	UMETA(DisplayName = "NATO_556"),
	HE_ROCKET	UMETA(DisplayName = "HE_ROCKET"),
	MAX   
};