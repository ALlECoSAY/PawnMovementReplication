// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "PreTest/Public/Munitions/Ammunitions/BaseHoloAmmunition.h"
#include "HoloMunitionComponent.generated.h"


class UInputMappingContext;
class UInputAction;
class ABaseHoloWeapon;
struct FBaseHoloAmmunition;


DECLARE_LOG_CATEGORY_EXTERN(LogHoloMunitionComponent, Log, All);

// AmmoChangedDelegate
DECLARE_MULTICAST_DELEGATE_OneParam(FOnAmmunitionAmountChangedSignature, FBaseHoloAmmunition);
// Current Weapon Changed Delegate
DECLARE_MULTICAST_DELEGATE_OneParam(FOnCurrentWeaponChangedSignature, ABaseHoloWeapon*);


UCLASS(Blueprintable, BlueprintType, ClassGroup=(Custom))
class PRETEST_API UHoloMunitionComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UHoloMunitionComponent();
	
	bool GiveWeapon(TObjectPtr<ABaseHoloWeapon> NewWeapon);

	UFUNCTION(NetMulticast, Reliable)
	void Multicast_ForceGiveWeapon(ABaseHoloWeapon* NewWeapon);
	
	UFUNCTION(Server, Reliable)
	void Server_SwapWeapon();
	UFUNCTION(Client, Reliable)
	void Multicast_UpdateCurrentWeapon(int32 NewCurrentIndex, ABaseHoloWeapon* NewCurrentWeapon);
    UFUNCTION()
	void StartFire();
    UFUNCTION()
	void StopFire();
    UFUNCTION(Server, Reliable)
	void Server_Reload();
	UFUNCTION(Client, Reliable)
	void Client_Reload(FBaseHoloAmmunition NewAmmunition);

	FORCEINLINE void SetCurrentWeaponIndex(int32 NewIndex)
	{
		CurrentWeaponIndex = NewIndex;
	}
	
protected:
	virtual void BeginPlay() override;

	UFUNCTION(Server, Reliable)
	void Server_DropWeapon();
	UFUNCTION(NetMulticast, Reliable)
	void Multicast_DropWeapon();
	
public:

	FOnAmmunitionAmountChangedSignature OnAmmunitionAmountChangedDelegate;

	FOnCurrentWeaponChangedSignature OnCurrentWeaponChangedDelegate;
	
protected:

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta=(AllowPrivateAccess="true"))
	int32 MaxWeapons = 2;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	int32 CurrentWeaponIndex = 0;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta=(AllowPrivateAccess="true"))
	TArray<TObjectPtr<ABaseHoloWeapon>> Weapons;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta=(AllowPrivateAccess="true"))
	TArray<FBaseHoloAmmunition> Ammunitions;

#pragma region Input

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta=(AllowPrivateAccess="true"), Category = "Input")
	TObjectPtr<UInputMappingContext> WeaponMappingContext;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta=(AllowPrivateAccess="true"), Category = "Input")
	TObjectPtr<UInputAction> DropWeaponAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta=(AllowPrivateAccess="true"), Category = "Input")
	TObjectPtr<UInputAction> SwapWeaponAction;

	//todo interactable/interactor component transfer ownership on interaction
	/*UPROPERTY(EditAnywhere, BlueprintReadOnly, meta=(AllowPrivateAccess="true"), Category = "Input")
	TObjectPtr<UInputAction> PickupAction;*/

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta=(AllowPrivateAccess="true"), Category = "Input")
	TObjectPtr<UInputAction> FireAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta=(AllowPrivateAccess="true"), Category = "Input")
	TObjectPtr<UInputAction> ReloadAction;

	
#pragma endregion
	
};
