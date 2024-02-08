// Fill out your copyright notice in the Description page of Project Settings.


#include "GameModes/MatchHoloGameMode.h"

#include "Components/HoloHealthComponent.h"
#include "Components/HoloMunitionComponent.h"
#include "GameFramework/PlayerStart.h"
#include "Kismet/GameplayStatics.h"
#include "Munitions/Weapons/BaseHoloWeapon.h"
#include "Pawns/HoloPawn.h"

void AMatchHoloGameMode::BeginPlay()
{
	Super::BeginPlay();

	check(StartupWeaponClass);
	//get all actors of class

	//timer give initial weapons
	FTimerHandle TimerHandle;
	GetWorldTimerManager().SetTimer(TimerHandle, this, &AMatchHoloGameMode::ScrapPawnsAndSpawnWeapons, 2.0f, false);

	UGameplayStatics::GetAllActorsOfClass(GetWorld(), APlayerStart::StaticClass(),PlayerStarts);
	
	
}

void AMatchHoloGameMode::ScrapPawnsAndSpawnWeapons()
{
	TArray<AActor*> Actors;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AHoloPawn::StaticClass(), Actors);

	for (auto Actor : Actors)
	{
		AHoloPawn* HoloPawn = CastChecked<AHoloPawn>(Actor);
		if (HoloPawn)
		{
			HoloPawns.Add(HoloPawn);

			HoloPawn->GetHealthComponent()->OnHealthChangedDelegate.AddUObject(this, &AMatchHoloGameMode::HandleDeath);
			
			//Spawn default weapon
			FActorSpawnParameters SpawnParams;
			SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
			SpawnParams.Owner = HoloPawn;		
			ABaseHoloWeapon* Weapon = GetWorld()->SpawnActor<ABaseHoloWeapon>(StartupWeaponClass, SpawnParams);
			HoloWeapons.Add(Weapon);
			
		}
	}
	
	FTimerHandle TimerHandle;
	GetWorldTimerManager().SetTimer(TimerHandle, this, &AMatchHoloGameMode::AssignInitialWeapons, 1.0f, false);
}

void AMatchHoloGameMode::AssignInitialWeapons()
{

	for (int32 i = 0; i < HoloPawns.Num(); ++i)
	{
		AHoloPawn* HoloPawn = HoloPawns[i];
		ABaseHoloWeapon* Weapon = HoloWeapons[i];
		if (HoloPawn && Weapon)
		{
			Weapon->SetOwner(HoloPawn);
			UHoloMunitionComponent* MunitionComponent = HoloPawn->GetMunitionComponent();
			if (MunitionComponent)
			{
				MunitionComponent->Multicast_ForceGiveWeapon(Weapon);
			}
		}
	}
	
}

void AMatchHoloGameMode::HandleDeath(float CurrentHealth, float MaxHealth, AActor* Damaged)
{
	AHoloPawn* HoloPawn = CastChecked<AHoloPawn>(Damaged);
	if (HoloPawn)
	{
		AController* PlayerController = CastChecked<APlayerController>(HoloPawn->GetController());

		HoloPawns.Remove(HoloPawn);
		HoloPawn->OnEndPlay.Remove(this, FName("HandleDeath"));
		FActorSpawnParameters SpawnParams;
		SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
		ABaseHoloWeapon* Weapon = GetWorld()->SpawnActor<ABaseHoloWeapon>(StartupWeaponClass, SpawnParams);
		
		FTimerHandle TimerHandle;
		const FTimerDelegate TimerDelegate = FTimerDelegate::CreateWeakLambda(this, [this, PlayerController, Weapon]()
		{
			PlayerController->DisableInput(nullptr);
			RestartPlayerAtPlayerStart(PlayerController, PlayerStarts[FMath::RandRange(0, PlayerStarts.Num() - 1)]);
			if(AHoloPawn* NewPawn = PlayerController->GetPawn<AHoloPawn>())
			{
				NewPawn->GetHealthComponent()->OnHealthChangedDelegate.AddUObject(this, &AMatchHoloGameMode::HandleDeath);
				if(Weapon)
				{
					Weapon->SetOwner(NewPawn);
					UHoloMunitionComponent* MunitionComponent = NewPawn->GetMunitionComponent();
					if (MunitionComponent)
					{
						MunitionComponent->Multicast_ForceGiveWeapon(Weapon);
					}
				}
				
			}
		});
		GetWorldTimerManager().SetTimer(TimerHandle, TimerDelegate, RespawnDelay, false);
	}
}
