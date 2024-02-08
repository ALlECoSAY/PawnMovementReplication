#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Munitions/Ammunitions/AmmunitionType.h"
#include "BaseHoloWeapon.generated.h"

class UArrowComponent;
class UBaseShootMode;
class UStaticMeshComponent;

// Ammo amount updated new diff
DECLARE_MULTICAST_DELEGATE_TwoParams(FOnAmmoAmountChangedSignature, int32, int32);

DECLARE_LOG_CATEGORY_EXTERN(LogBaseHoloWeapon, Log, All);

UCLASS()
class PRETEST_API ABaseHoloWeapon : public AActor
{
	GENERATED_BODY()
	
public:	

	ABaseHoloWeapon();

	void TryFire();

	virtual void Fire();

	UFUNCTION(Server, Reliable)
	virtual void Server_Fire();

#pragma region GET/SET

	FORCEINLINE UBaseShootMode* GetCurrentShootMode() const { return CurrentShootMode; }
	
	FORCEINLINE EAmmunition GetAmmunitionType() const { return AmmunitionType; }

	FORCEINLINE int32 GetCurrentAmmunition() const { return CurrentAmmunition; }

	FORCEINLINE int32 GetMaxAmmunitionCapacity() const { return MaxAmmunitionCapacity; }
	
#pragma endregion

	
protected:
	virtual void BeginPlay() override;


	UFUNCTION(Client, Reliable)
	void Client_SetCurrentAmmunition(int32 NewAmmunitionAmount);

	UFUNCTION(NetMulticast, Unreliable)
	void Multicast_PlayFireEffects();

public:
	FOnAmmoAmountChangedSignature OnAmmunitionAmountChanged;

protected:

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta=(AllowPrivateAccess="true"))
	TObjectPtr<UStaticMeshComponent> MeshComponent;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta=(AllowPrivateAccess="true"))
	TObjectPtr<UArrowComponent> MuzzleAimArrowComponent;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta=(AllowPrivateAccess="true"))
	int32 MaxAmmunitionCapacity = 30;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta=(AllowPrivateAccess="true"))
	int32 CurrentAmmunition = 0;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta=(AllowPrivateAccess="true"))
	EAmmunition AmmunitionType = EAmmunition::NATO_556;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta=(AllowPrivateAccess="true"))
	float DamageAmount = 10.f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta=(AllowPrivateAccess="true"))
	TArray<TSubclassOf<UBaseShootMode>> ShootModesClasses;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta=(AllowPrivateAccess="true"))
	TObjectPtr<UBaseShootMode> CurrentShootMode;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta=(AllowPrivateAccess="true"))
	TArray<TObjectPtr<UBaseShootMode>> ShootModes;
	
};
