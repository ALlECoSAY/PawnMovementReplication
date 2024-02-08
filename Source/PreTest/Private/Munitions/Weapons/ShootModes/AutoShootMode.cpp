#include "Munitions/Weapons/ShootModes/AutoShootMode.h"


void UAutoShootMode::HandleFireStart()
{
	const UWorld* World = GetWorld();
	check(World);

	const auto FireDelegate = FTimerDelegate::CreateWeakLambda(
		this,
		[this] { OnShouldShootDelegate.ExecuteIfBound(); });

	World->GetTimerManager().SetTimer(FireHandle, FireDelegate, ShootRate, true, 0.f);
}

void UAutoShootMode::HandleFireEnd()
{
	const UWorld* World = GetWorld();
	check(World);

	World->GetTimerManager().ClearTimer(FireHandle);
}

bool UAutoShootMode::IsFiring() const
{
	bool bIsFiring = false;
	
	if (const UWorld* World = GetWorld())
	{
		bIsFiring = World->GetTimerManager().IsTimerActive(FireHandle);
	}
	
	return bIsFiring;
}
