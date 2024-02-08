// Fill out your copyright notice in the Description page of Project Settings.


#include "PreTest/Public/Components/HoloMunitionComponent.h"

#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "Munitions/Weapons/ShootModes/BaseShootMode.h"
#include "Pawns/HoloPawn.h"
#include "PreTest/Public/Munitions/Weapons/BaseHoloWeapon.h"


DEFINE_LOG_CATEGORY(LogHoloMunitionComponent);

// Sets default values for this component's properties
UHoloMunitionComponent::UHoloMunitionComponent()
{
	PrimaryComponentTick.bCanEverTick = true;

	Ammunitions.SetNum(static_cast<int32>(EAmmunition::MAX));

	Weapons.SetNum(1);
	
}




void UHoloMunitionComponent::BeginPlay()
{
	Super::BeginPlay();

	const APawn* Owner = GetOwner<APawn>();
	check(Owner);
	
	if (!Owner->IsLocallyControlled())
	{
		return;
	}

	const APlayerController* PlayerController = Cast<APlayerController>(Owner->Controller);
	check(PlayerController);

#pragma region Input setup

	UEnhancedInputLocalPlayerSubsystem* Subsystem =
		ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer());
	if (!Subsystem)
	{
		UE_LOG(LogTemp, Error, TEXT("UPlayerWeaponComponent works only with UEnhancedInputLocalPlayerSubsystem"));
		return;
	}
	
	Subsystem->AddMappingContext(WeaponMappingContext, 0);
	

	UEnhancedInputComponent* EnhancedInputComponent =
		CastChecked<UEnhancedInputComponent>(PlayerController->InputComponent);

	
	EnhancedInputComponent->BindAction(DropWeaponAction, ETriggerEvent::Started, this, &ThisClass::Server_DropWeapon);
	EnhancedInputComponent->BindAction(SwapWeaponAction, ETriggerEvent::Started, this, &ThisClass::Server_SwapWeapon);
	EnhancedInputComponent->BindAction(FireAction, ETriggerEvent::Started, this, &ThisClass::StartFire);
	EnhancedInputComponent->BindAction(FireAction, ETriggerEvent::Canceled, this, &ThisClass::StopFire);
	EnhancedInputComponent->BindAction(ReloadAction, ETriggerEvent::Started, this, &ThisClass::Server_Reload);
	
#pragma endregion	
}




void UHoloMunitionComponent::Server_DropWeapon_Implementation()
{
	if(!Weapons[CurrentWeaponIndex])
	{
		UE_LOG(LogHoloMunitionComponent, Warning, TEXT("No weapon to drop"));
		return;
	}
	Multicast_DropWeapon_Implementation();
}

void UHoloMunitionComponent::Multicast_DropWeapon_Implementation()
{
	const auto DroppedWeapon = Weapons[CurrentWeaponIndex];
	if(IsValid(DroppedWeapon))
	{
		FDetachmentTransformRules DetachmentTransformRules(EDetachmentRule::KeepWorld, true);
		DroppedWeapon->DetachFromActor(DetachmentTransformRules);
	}
	Weapons[CurrentWeaponIndex] = nullptr;
}


bool UHoloMunitionComponent::GiveWeapon(TObjectPtr<ABaseHoloWeapon> NewWeapon)
{
	if (Weapons.Num() < MaxWeapons)
	{
		Weapons.Add(NewWeapon);
		return true;
	}
	return false;
}

void UHoloMunitionComponent::Multicast_ForceGiveWeapon_Implementation(ABaseHoloWeapon* NewWeapon)
{
	check(NewWeapon);
	if (Weapons.Num() < MaxWeapons)
	{
		CurrentWeaponIndex = Weapons.Add(NewWeapon);
		NewWeapon->SetActorHiddenInGame(false);//todo true
		NewWeapon->SetActorEnableCollision(false);
		NewWeapon->AttachToActor(GetOwner(), FAttachmentTransformRules::SnapToTargetNotIncludingScale, FName("WeaponSocket"));
	}

	if(GetOwnerRole()==ROLE_Authority)
	{
		NewWeapon->SetOwner(GetOwner());
	}
}



void UHoloMunitionComponent::Server_SwapWeapon_Implementation()
{
	if(Weapons[CurrentWeaponIndex])
	{
		Weapons[CurrentWeaponIndex]->SetActorHiddenInGame(true);
	}
	SetCurrentWeaponIndex((CurrentWeaponIndex + 1) % Weapons.Num());
	if(Weapons[CurrentWeaponIndex])
	{
		Weapons[CurrentWeaponIndex]->SetActorHiddenInGame(false);
		OnCurrentWeaponChangedDelegate.Broadcast(Weapons[CurrentWeaponIndex]);
	}
	Multicast_UpdateCurrentWeapon_Implementation(CurrentWeaponIndex, Weapons[CurrentWeaponIndex]);
	
}

void UHoloMunitionComponent::Multicast_UpdateCurrentWeapon_Implementation(int32 NewCurrentIndex, ABaseHoloWeapon* NewCurrentWeapon)
{
	if(IsValid(Weapons[CurrentWeaponIndex]))
	{
		Weapons[CurrentWeaponIndex]->SetActorHiddenInGame(true);
	}
	SetCurrentWeaponIndex( NewCurrentIndex);
	Weapons[CurrentWeaponIndex] = NewCurrentWeapon;
	if(IsValid(Weapons[CurrentWeaponIndex]))
	{
		Weapons[CurrentWeaponIndex]->SetActorHiddenInGame(false);
	}
	OnCurrentWeaponChangedDelegate.Broadcast(Weapons[CurrentWeaponIndex]);
}

void UHoloMunitionComponent::StartFire()
{
	if(!Weapons[CurrentWeaponIndex])
	{
		UE_LOG(LogHoloMunitionComponent, Warning, TEXT("No weapon to fire"));
		return;
	}
	Weapons[CurrentWeaponIndex]->TryFire();
}

void UHoloMunitionComponent::StopFire()
{
}

void UHoloMunitionComponent::Server_Reload_Implementation()
{

	if(!Weapons[CurrentWeaponIndex])
	{
		UE_LOG(LogHoloMunitionComponent, Warning, TEXT("No weapon to reload"));
		return;
	}
	
	const ABaseHoloWeapon* CurrentWeapon = Weapons[CurrentWeaponIndex];
	
	const int32 AmmoToReload = CurrentWeapon->GetMaxAmmunitionCapacity() - CurrentWeapon->GetCurrentAmmunition();
	if(!AmmoToReload)
	{
		UE_LOG(LogHoloMunitionComponent, Warning, TEXT("No need to reload"));
		return;
	}
	
	EAmmunition AmmoType = CurrentWeapon->GetAmmunitionType();
	const int32 AmmoIndex = static_cast<int32>(AmmoType);

	const int32 AmmoToTake = FMath::Min(AmmoToReload, Ammunitions[AmmoIndex].CurrentNumber);
	
	Ammunitions[AmmoIndex].CurrentNumber -= AmmoToTake;
	
	OnAmmunitionAmountChangedDelegate.Broadcast(Ammunitions[AmmoIndex]);

	if(GetOwner()->GetRemoteRole()==ROLE_AutonomousProxy)
	{
		Client_Reload(Ammunitions[AmmoIndex]);
	}
	
	
}

void UHoloMunitionComponent::Client_Reload_Implementation(FBaseHoloAmmunition NewAmmunition)
{
	const int32  AmmoIndex = static_cast<int32>(NewAmmunition.Type);
	Ammunitions[AmmoIndex] = NewAmmunition;
	
	OnAmmunitionAmountChangedDelegate.Broadcast(Ammunitions[AmmoIndex]);
}