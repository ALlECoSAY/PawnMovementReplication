#include "PreTest/Public/Munitions/Weapons/BaseHoloWeapon.h"

#include "Components/ArrowComponent.h"
#include "Munitions/Weapons/ShootModes/SingleShootMode.h"

DEFINE_LOG_CATEGORY(LogBaseHoloWeapon);

ABaseHoloWeapon::ABaseHoloWeapon()
{
	PrimaryActorTick.bCanEverTick = false;

	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("RootComponent"));

	MeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComponent"));
	MeshComponent->SetupAttachment(RootComponent);

	MuzzleAimArrowComponent = CreateDefaultSubobject<UArrowComponent>(TEXT("MuzzleAimArrowComponent"));
	MuzzleAimArrowComponent->SetupAttachment(MeshComponent);

	bReplicates = true;
	bAlwaysRelevant = true;

	//Overlap World Dynamic
	MeshComponent->SetCollisionProfileName(TEXT("OverlapAllDynamic"));
	
}


void ABaseHoloWeapon::TryFire()
{
	if(!CurrentAmmunition)
	{
		UE_LOG(LogBaseHoloWeapon, Warning, TEXT("No ammunition loaded"));
		return;
	}
	if (CurrentShootMode)
	{
		CurrentShootMode->HandleFireStart();
	}
}

void ABaseHoloWeapon::BeginPlay()
{
	Super::BeginPlay();

	check(ShootModesClasses.Num());
	
	for (auto ShootModeClass : ShootModesClasses)
	{
		const auto NewShootMode = NewObject<UBaseShootMode>(this, ShootModeClass);
		ShootModes.Add(NewShootMode);
		NewShootMode->OnShouldShootDelegate.BindUObject(this, &ThisClass::Fire);
	}
	CurrentShootMode = ShootModes[0];
}

void ABaseHoloWeapon::Multicast_PlayFireEffects_Implementation()
{
	UE_LOG(LogBaseHoloWeapon, Warning, TEXT("Multicast_PlayFireEffects_Implementation" )); 
}

void ABaseHoloWeapon::Client_SetCurrentAmmunition_Implementation(int32 NewAmmunitionAmount)
{
	OnAmmunitionAmountChanged.Broadcast(NewAmmunitionAmount, NewAmmunitionAmount - CurrentAmmunition );
	CurrentAmmunition = NewAmmunitionAmount;
}


void ABaseHoloWeapon::Fire()
{
	if (!CurrentAmmunition)
	{
		UE_LOG(LogBaseHoloWeapon, Warning, TEXT("No ammunition loaded"));
		return;
	}

	if(GetLocalRole()==ROLE_AutonomousProxy)
	{
		CurrentAmmunition--;
	}
}

void ABaseHoloWeapon::Server_Fire_Implementation()
{
	if(!CurrentAmmunition) {
		UE_LOG(LogBaseHoloWeapon, Warning, TEXT("No ammunition loaded"));
		return;
	}
	CurrentAmmunition--;
	Multicast_PlayFireEffects();
}

