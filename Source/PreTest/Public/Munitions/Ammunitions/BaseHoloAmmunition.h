#pragma once

#include "CoreMinimal.h"
#include "AmmunitionType.h"
#include "GameFramework/Actor.h"
#include "BaseHoloAmmunition.generated.h"

USTRUCT(Blueprintable, BlueprintType)
struct  PRETEST_API FBaseHoloAmmunition
{
	GENERATED_BODY()
	
	FBaseHoloAmmunition();


	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta=(AllowPrivateAccess="true"))
	int32 CurrentNumber = 90;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta=(AllowPrivateAccess="true"))
	int32 MaxNumber = 90;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta=(AllowPrivateAccess="true"))
	EAmmunition Type = EAmmunition::NONE;
	
};
