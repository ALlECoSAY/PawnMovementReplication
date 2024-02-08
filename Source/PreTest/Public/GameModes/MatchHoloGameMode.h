#pragma once

#include "CoreMinimal.h"
#include "BaseHoloGameMode.h"
#include "MatchHoloGameMode.generated.h"

class APlayerStart;
class AHoloPawn;
class ABaseHoloWeapon;


UCLASS()
class PRETEST_API AMatchHoloGameMode : public ABaseHoloGameMode
{
	GENERATED_BODY()

protected:
	virtual void BeginPlay() override;

	UFUNCTION()
	void ScrapPawnsAndSpawnWeapons();
	
	UFUNCTION()
	void AssignInitialWeapons();

	UFUNCTION()
	void HandleDeath( float CurrentHealth, float MaxHealth, AActor* DamagedActor );

protected:

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "GameMode", meta = (AllowPrivateAccess = "true"))
	float RespawnDelay = 4.f;
	
	UPROPERTY(EditDefaultsOnly, Category = "GameMode")
	TSubclassOf<ABaseHoloWeapon> StartupWeaponClass;

	UPROPERTY(Transient)
	TArray<TObjectPtr<AHoloPawn>> HoloPawns;

	UPROPERTY(Transient)
	TArray<TObjectPtr<ABaseHoloWeapon>> HoloWeapons;

	UPROPERTY(Transient)
	TArray<TObjectPtr<AActor>> PlayerStarts;
};
