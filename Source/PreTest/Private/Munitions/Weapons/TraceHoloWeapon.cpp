#include "Munitions/Weapons/TraceHoloWeapon.h"

#include "Components/ArrowComponent.h"
#include "Components/HoloHealthComponent.h"
#include "Interfaces/Damagable.h"


void ATraceHoloWeapon::Server_PerformTrace_Implementation()
{
	const FHitResult Hit = TraceMuzzle();
	if(const AActor* HittedActor = Hit.GetActor())
	{
		if(const IDamagable* Damagable = Cast<IDamagable>(HittedActor))
		{
			Damagable->GetHealthComponent()->Damage(DamageAmount);
		}
	}
	else
	{
		UE_LOG(LogBaseHoloWeapon, Warning, TEXT("SERVER:No actor was hitted"));
	}
}

void ATraceHoloWeapon::Fire()
{
	Super::Fire();
	
	const FHitResult Hit = TraceMuzzle();
	if(const AActor* HittedActor = Hit.GetActor())
	{
		if(const IDamagable* Damagable = Cast<IDamagable>(HittedActor))
		{
			Server_PerformTrace();			
		}
	}
}

FHitResult ATraceHoloWeapon::TraceMuzzle()
{
	UE_LOG(LogBaseHoloWeapon, Warning, TEXT("TraceMuzzle" ));
	const FVector StartLocation = MuzzleAimArrowComponent->GetComponentLocation();
	const FVector EndLocation = StartLocation + MuzzleAimArrowComponent->GetForwardVector() * TraceDistance;

	FCollisionQueryParams CollisionQueryParams;
	CollisionQueryParams.AddIgnoredActor(this);
	CollisionQueryParams.bTraceComplex = true;
	//debug trace todo remove
	DrawDebugLine(GetWorld(), StartLocation, EndLocation, FColor::Red, false, 1.f, 0, 1.f);
	//perform trace
	FHitResult HitResult;
	GetWorld()->LineTraceSingleByChannel(HitResult, StartLocation, EndLocation, ECC_Visibility, CollisionQueryParams);
	return HitResult;	
}